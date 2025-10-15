defmodule Gm.Codec.Decode do
  @moduledoc """
  Implement an decoder according to the JAM spec for a generic `Codec` schema type.
  """
  use Codec.Decoder
  require Logger

  @impl true
  def decode_generic(data, {:maybe, schema}) do
    case data do
      <<0::8, rest::binary>> ->
        {nil, rest}

      <<1::8, rest::binary>> ->
        decode_generic(rest, schema)

      <<>> ->
        throw(Scale.DecodeError.missing_data({:maybe, schema}, 1, <<>>))

      other ->
        throw(
          Scale.DecodeError.invalid(
            {:maybe, schema},
            other
          )
        )
    end
  end

  def decode_generic(binary, {:result, schema1, schema2}) do
    {choice, rest} = Scale.decode_u8(binary)

    case choice do
      0 ->
        {value, rest} = decode_generic(rest, schema1)
        {{:ok, value}, rest}

      1 ->
        {value, rest} = decode_generic(rest, schema2)
        {{:error, value}, rest}
    end
  end

  def decode_generic(binary, :void), do: {nil, binary}
  def decode_generic(binary, :bool), do: Scale.decode_bool(binary)
  def decode_generic(binary, :u8), do: Scale.decode_u8(binary)
  def decode_generic(binary, :u16), do: Scale.decode_u16(binary)
  def decode_generic(binary, :u24), do: Scale.decode_u24(binary)
  def decode_generic(binary, :u32), do: Scale.decode_u32(binary)
  def decode_generic(binary, :u64), do: Scale.decode_u64(binary)
  def decode_generic(binary, {:custom, _enc, dec}) when is_function(dec, 1), do: dec.(binary)
  def decode_generic(binary, :general_int), do: Scale.decode_general_integer(binary)
  def decode_generic(binary, :binary), do: Scale.decode_binary(binary)
  def decode_generic(binary, :string), do: Scale.decode_binary(binary)

  def decode_generic(data, {:bounded_binary, len}) when is_function(len, 0),
    do: decode_generic(data, {:bounded_binary, len.()})

  def decode_generic(data, {:bounded_binary, len}), do: Scale.decode_bounded_binary(data, len)

  def decode_generic(data, :fixed_binary), do: Scale.decode_fixed_binary_no_size(data)

  def decode_generic(data, {:fixed_binary, len}) when is_function(len, 0),
    do: Scale.decode_fixed_binary(data, len.())

  def decode_generic(data, {:fixed_binary, len}), do: Scale.decode_fixed_binary(data, len)

  def decode_generic(data, {:fixed_bitstring, len}) when is_function(len, 0),
    do: Scale.decode_fixed_bitstring(data, len.())

  def decode_generic(data, {:fixed_bitstring, len}), do: Scale.decode_fixed_bitstring(data, len)

  def decode_generic(binary, {:bounded_list, inner_type, max_size})
      when is_function(max_size, 0) do
    decode_generic(binary, {:bounded_list, inner_type, max_size.()})
  end

  def decode_generic(binary, {:bounded_list, inner_type, max_size}) do
    Scale.decode_bounded_list(binary, max_size, fn data ->
      Gm.Codec.Decode.decode_generic(data, inner_type)
    end)
  end

  def decode_generic(binary, {:list, inner_type}) do
    Scale.decode_list(binary, fn data ->
      Gm.Codec.Decode.decode_generic(data, inner_type)
    end)
  end

  def decode_generic(binary, {:array, inner_type, max_size}) when is_function(max_size, 0) do
    decode_generic(binary, {:array, inner_type, max_size.()})
  end

  def decode_generic(binary, {:array, inner_type, max_size}) do
    Scale.decode_list_fixed_len(
      binary,
      fn data ->
        Gm.Codec.Decode.decode_generic(data, inner_type)
      end,
      max_size
    )
  end

  def decode_generic(binary, {:dict, {_key_name, key_type}, {_value_name, value_type}}) do
    Scale.decode_map_all_kvs(
      binary,
      fn k -> decode_generic(k, key_type) end,
      fn v -> decode_generic(v, value_type) end
    )
  end

  def decode_generic(data, {:set, schema}) do
    Scale.decode_set(data, fn data -> decode_generic(data, schema) end)
  end

  def decode_generic(data, {:fixed_set, schema, size}) when is_function(size, 0) do
    decode_generic(data, {:fixed_set, schema, size.()})
  end

  def decode_generic(data, {:fixed_set, schema, size}) do
    Scale.decode_fixed_set(data, size, fn data -> decode_generic(data, schema) end)
  end

  def decode_generic(data, {:container, module, :indirect, opts}),
    do: decode_generic(data, {:container, module, module.schema(), opts})

  def decode_generic(data, {:container, module, schema, _opts}) do
    {map, rest} = decode_map(module, data, schema)
    {struct!(module, map), rest}
  end

  defp decode_map(_module, data, []), do: {[], data}

  defp decode_map(module, data, [{key, :custom} | rest]) do
    {value, data} = decode_container_custom(module, key, data)
    {map, data} = decode_map(module, data, rest)
    {[{key, value} | map], data}
  end

  # TODO: Can't decode tickets in fuzzing_test.
  defp decode_map(module, data, [{key, schema} | rest]) do
    {value, data} = decode_generic(data, schema)

    # Un-comment for debug logging.
    # trace_field_decoding(module, key, schema, value)

    {map, data} = decode_map(module, data, rest)
    {[{key, value} | map], data}
  end

  # We dont log containers since we already log all their members.
  # defp trace_field_decoding(_module, _key, {:container, _, _, _}, _value), do: :ok
  # defp trace_field_decoding(module, key, schema, value) do
  #  short_schema = inspect(schema) |> String.slice(0, 50) |> String.trim_leading(":")
  #  short_value = inspect(value) |> String.slice(0, 200)
  #  mod_name = module |> Atom.to_string() |> String.trim_leading("Elixir.")
  #  Logger.debug("Decoding '#{mod_name}.#{key}' as '#{short_schema}': #{short_value}")
  # end

  defp decode_container_custom(container, field, value) do
    if not Kernel.function_exported?(container, :decode_custom_codec, 2) do
      raise ArgumentError,
            "Please define function `#{container}.decode_custom_codec/2` to make the CODEC custom decoding work for field `#{field}`."
    end

    container.decode_custom_codec(field, value)
  end
end

