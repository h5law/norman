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

    const flags = [_][]const u8{
        "-I./include/",
    };
    const cflags = flags ++ [_][]const u8{
        "-nostdinc",
        "-nostdlib",
        "-fno-builtin",
        "-ffreestanding",
        "-fno-stack-protector",
    };

    const nlibc = b.addLibrary(.{
        .root_module = ".",
        .linkage = .static,
        .name = "nlibc",
    });

    var nlibcSources = std.ArrayList([]const u8).init(b.allocator);
    {
        var dir = try std.fs.cwd().openDir("src", .{ .iterate = true });

        var walker = try dir.walk(b.allocator);
        defer walker.deinit();

        const allowed_dirs = [_][]const u8{
            "stdio", "stdlib", "string", "sys",
        };
        while (try walker.next()) |entry| {
            const ext = std.fs.path.extension(entry.basename);
            const include_file = for (allowed_dirs) |e| {
                if (std.mem.eql(u8, ext, e))
                    break true;
            } else false;
            if (include_file) {
                try nlibcSources.append(b.pathJoin("src", entry.path));
            }
        }
    }

    nlibc.addCSourceFiles(nlibcSources.items, &cflags);

    b.installArtifact(nlibc);

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
        .options = &cflags,
    });

    demo.linkLibrary(nlibc);

    b.installArtifact(demo);

    // Add a run step for the demo
    const run_cmd = b.addRunArtifact(demo);
    run_cmd.addArg("src/system.h");
    const run_step = b.step("run", "Run the application");
    run_step.dependOn(&run_cmd.step);
}
