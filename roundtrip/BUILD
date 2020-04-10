cc_library(
    name = "build-others",
    srcs = (["roundtrip_udp.cc"]),
    deps = ["//main:build-other-files"],
)

cc_binary(
    name = "roundtrip",
    deps = [":build-others"],
)
