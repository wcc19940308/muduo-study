cc_library(
    name = "build-client",
    srcs = (["echo_client.cc"]),
    deps = ["//main:build-other-files"],
)

cc_binary(
    name = "echo-client",
    deps = [":build-client"],
)
