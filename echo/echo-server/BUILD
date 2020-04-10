
cc_library(
    name = "build-server",
    srcs = (["echo.cc"]),
    deps = ["//main:build-other-files"],
)

cc_binary(
    name = "echo-server",
    deps = [":build-server"],
)
