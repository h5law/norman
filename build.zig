const std = @import("std");

pub fn build(b: *std.Build) void {
    // Get target and optimization mode
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{
        .preferred_optimize_mode = .Debug,
    });

    // Validate supported architectures
    switch (target.result.cpu.arch) {
        .aarch64 => {},
        else => {
            std.debug.print("Unsupported arch: {?}. Only arm64/aarch64 is currently supported.\n", .{target.result.cpu.arch});
            std.process.exit(1);
        },
    }

    const nlibc = b.addStaticLibrary(.{
        .name = "nlibc",
        .target = target,
        .optimize = optimize,
        .link_libc = false,
    });

    nlibc.addCSourceFiles(.{
        .files = &[_][]const u8{
            "src/syscall-arm64.S",
            "src/system.c",
            "src/errno.c",
            "src/stdio.c",
            "src/mem.c",
            "src/string.c",
        },
        .flags = &[_][]const u8{
            "-nostdinc",
            "-fno-builtin",
            "-ffreestanding",
            "-fno-stack-protector",
        },
    });

    // Create the demo executable
    const demo = b.addExecutable(.{
        .name = "demo",
        .target = target,
        .optimize = optimize,
        .link_libc = false,
    });

    // Set source files and build flags
    demo.addCSourceFiles(.{
        .files = &[_][]const u8{
            "tests/syscalls.c",
        },
        .flags = &[_][]const u8{
            "-nostdinc",
            "-fno-builtin",
            "-ffreestanding",
            "-fno-stack-protector",
        },
    });

    demo.linkLibrary(nlibc);

    b.installArtifact(demo);

    // Add a run step for the demo
    const run_cmd = b.addRunArtifact(demo);
    run_cmd.addArg("src/system.h");
    const run_step = b.step("run", "Run the application");
    run_step.dependOn(&run_cmd.step);
}
