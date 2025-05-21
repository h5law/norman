const std = @import("std");

pub fn build(b: *std.Build) void {
    // Get target and optimization mode from command line arguments
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{
        .preferred_optimize_mode = .Debug,
    });

    // Validate architecture - check if target is arm64/aarch64
    switch (target.result.cpu.arch) {
        .aarch64, .arm => {},
        else => {
            std.debug.print("Unsupported arch: {?}. Only arm64/aarch64 is currently supported.\n", .{target.result.cpu.arch});
            std.process.exit(1);
        },
    }

    // Create an executable
    const exe = b.addExecutable(.{
        .name = "demo",
        .target = target,
        .optimize = optimize,
        .link_libc = false,
    });

    // Set build flags corresponding to the CMake options
    exe.addCSourceFiles(.{
        .files = &[_][]const u8{
            "src/syscall-arm64.S",
            "src/system.c",
            "src/errno.c",
            "src/stdio.c",
            "src/mem.c",
            "src/string.c",
            "tests/syscalls.c",
        },
        .flags = &[_][]const u8{
            "-nostdinc",
            "-fno-builtin",
            "-ffreestanding",
            "-fno-stack-protector",
        },
    });

    const build_dir = b.fmt("{s}-{s}", .{ @tagName(optimize), @tagName(target.result.cpu.arch) });

    // Add the executable as the default install step
    const install = b.addInstallArtifact(exe, .{
        .dest_dir = .{
            .override = .{ .custom = build_dir },
        },
    });
    b.default_step.dependOn(&install.step);

    // Add a run step
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.addArg("src/system.h");

    // Add "run" step for easier testing
    const run_step = b.step("run", "Run the application");
    run_step.dependOn(&run_cmd.step);
}
