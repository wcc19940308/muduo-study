cc_library(
    name = "build-all-the-files",
    srcs = glob(
    ["*.cc"], 
    exclude = ["main.cc"]
    ),
    hdrs = glob(["*.h"]),
    linkopts = ["-lboost_program_options"],
)

cc_binary(
    name = "TTCP",
    srcs = ["main.cc"],
    deps = [":build-all-the-files"],
)
