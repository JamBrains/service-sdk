defmodule Gm.Codec.Encode do
  @moduledoc """
  Implement an encoder according to the JAM spec for a generic `Codec` schema type.
  """
  use Codec.Encoder

  @impl true
  @spec encode_generic(any(), Container.schema()) :: iodata()
  def encode_generic(nil, :void), do: <<>>
  def encode_generic(value, :u8), do: Scale.encode_u8(value)
  def encode_generic(value, :u16), do: Scale.encode_u16(value)
  def encode_generic(value, :u24), do: Scale.encode_u24(value)
  def encode_generic(value, :u32), do: Scale.encode_u32(value)
  def encode_generic(value, :u64), do: Scale.encode_u64(value)
  def encode_generic(value, :general_int), do: Scale.encode_general_integer(value)
  def encode_generic(value, :bool), do: Scale.encode_bool(value)
  def encode_generic(value, {:custom, enc, _dec}) when is_function(enc, 1), do: enc.(value)
  def encode_generic(value, :binary), do: Scale.encode_binary(value)
  def encode_generic(value, :string), do: Scale.encode_binary(value)
  def encode_generic(value, {:fixed_binary, _}), do: Scale.encode_fixed_binary(value)

  def encode_generic(value, {:fixed_bitstring, size_fn}) when is_function(size_fn, 0) do
    encode_generic(value, {:fixed_bitstring, size_fn.()})
  end

  def encode_generic(value, {:fixed_bitstring, _}) when is_binary(value),
    do: Scale.encode_fixed_binary(value)

  def encode_generic(value, {:fixed_bitstring, _}) when is_bitstring(value) do
    Scale.encode_fixed_bitstring(value)
  end

  def encode_generic(value, {:bounded_binary, _}), do: Scale.encode_bounded_binary(value)

  def encode_generic(nil, {:maybe, _}), do: <<0::8>>
  def encode_generic(value, {:maybe, schema}), do: [<<1::8>> | encode_generic(value, schema)]

  def encode_generic({:left, value}, {:either, left_schema, _}),
    do: [<<0::8>> | encode_generic(value, left_schema)]

  def encode_generic({:right, value}, {:either, _, right_schema}),
    do: [<<1::8>> | encode_generic(value, right_schema)]

  def encode_generic(list, {:list, inner_type}) do
    encode_list(list, inner_type)
  end

  def encode_generic(array, {:bounded_list, inner_type, _}) do
    encode_list(array, inner_type)
  end

  def encode_generic(array, {:array, inner_type, _}) do
    encode_array(array, inner_type)
  end

  def encode_generic(map, {:dict, {_key_name, key_type}, {_value_name, value_type}}) do
    Scale.encode_map_all_kvs(
      map,
      fn k -> encode_generic(k, key_type) end,
      fn v -> encode_generic(v, value_type) end
    )
  end

  def encode_generic(set, {:set, inner_type}) do
    Scale.encode_set(set, &encode_generic(&1, inner_type))
  end

  def encode_generic(set, {:fixed_set, inner_type, _}) do
    Scale.encode_fixed_set(set, &encode_generic(&1, inner_type))
  end

  def encode_generic(container, {:container, module, :indirect, opts}),
    do: encode_generic(container, {:container, module, module.schema(), opts})

  def encode_generic(container, {:container, module, schemata, _opts}) do
    for {key, schema} <- schemata do
      value = Map.fetch!(container, key)

      if schema == :custom do
        encode_container_custom(module, key, value)
      else
        encode_generic(value, schema)
      end
    end
  end

  def encode_list(list, inner_type), do: Scale.encode_list(list, &encode_generic(&1, inner_type))

  def encode_array(list, inner_type),
    do: Scale.encode_list_fixed_len(list, &encode_generic(&1, inner_type))

  defp encode_container_custom(container, field, value) do
    Code.ensure_loaded!(container)

    if not Kernel.function_exported?(container, :encode_custom_codec, 2) do
      raise ArgumentError,
            "Please define function `#{container}.encode_custom_codec/2` to make the CODEC custom decoding work for field `#{field}`."
    end

    container.encode_custom_codec(field, value)
  end
end

