cc_library(
    name = "build-all-the-files",
    srcs = glob(["*.cc"]),
    hdrs = glob(["*.h"]),
    linkopts = ["-lpthread"],
)

cc_binary(
    name = "echo",
    deps = [":build-all-the-files"],
)
