  typedstruct enforce: true do
    codec_field(:anchor, Types.h256())
    codec_field(:posterior_state_root, Types.h256())
    codec_field(:posterior_beefy_root, Types.h256())
    codec_field(:lookup_anchor, Types.h256())
    codec_field(:lookup_timeslot, Types.uint32())
    codec_field(:prerequisite_work_package_hashes, MapSet.t(Types.h256()))
  end

