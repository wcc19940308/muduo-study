cc_library(
    name = "build-other-files",
    srcs = glob(["*.cc"],),
    hdrs = glob(["*.h"]),
    linkopts = ["-lpthread"],
    visibility = ["//echo-client:__pkg__", "//echo-server:__pkg__", "//roundtrip:__pkg__"],
)