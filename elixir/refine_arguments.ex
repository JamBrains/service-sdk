defmodule PVM.Invocations.Refine.Arguments do
  @moduledoc """
  Arguments passed in to the refine PVM invocation.
  """
  use Container.Derive

  alias Spec.Types

  typedstruct enforce: true do
    codec_field(:core_index, Types.uint32(), as: :general_int)
    codec_field(:work_item_index, Types.uint32(), as: :general_int)
    codec_field(:service_id, Types.uint32(), as: :general_int)
    codec_field(:work_payload, binary())
    codec_field(:work_package_hash, Types.h256())
  end
end
