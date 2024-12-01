const std = @import("std");

// Build configuration for Zig project.
pub fn build(b: *std.Build) void {
    // Standard target options allows the person running `zig build` to choose
    // what target to build for. Here we do not override the defaults, which
    // means any target is allowed, and the default is native.
    const target = b.standardTargetOptions(.{});

    // Set the build optimization to Debug (for debugging with symbols).
    const optimize = b.standardOptimizeOption(.{ .mode = .Debug });

    // Static library build (if needed)
    const lib = b.addStaticLibrary(.{
        .name = "my_first_zig_lib",
        .root_source_file = b.path("src/root.zig"),
        .target = target,
        .optimize = optimize,
    });

    // Install the library as an artifact (optional for this case).
    b.installArtifact(lib);

    // Create executable with debug symbols enabled.
    const exe = b.addExecutable(.{
        .name = "my_first_zig", // Name of the executable.
        .root_source_file = b.path("src/main.zig"), // Main Zig file.
        .target = target,
        .optimize = optimize, // Enable debug mode (includes debug symbols).
    });

    // Install the executable as an artifact.
    b.installArtifact(exe);

    // Run the executable after building it.
    const run_cmd = b.addRunArtifact(exe);

    // Make sure the executable runs from the install directory.
    run_cmd.step.dependOn(b.getInstallStep());

    // Allow passing arguments to the executable (e.g., `zig build run -- arg1 arg2`).
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    // Expose a "run" step in the build system.
    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);

    // Create unit tests for the static library.
    const lib_unit_tests = b.addTest(.{
        .root_source_file = b.path("src/root.zig"),
        .target = target,
        .optimize = optimize,
    });

    const run_lib_unit_tests = b.addRunArtifact(lib_unit_tests);

    // Create unit tests for the executable.
    const exe_unit_tests = b.addTest(.{
        .root_source_file = b.path("src/main.zig"),
        .target = target,
        .optimize = optimize,
    });

    const run_exe_unit_tests = b.addRunArtifact(exe_unit_tests);

    // Expose a "test" step to run the unit tests.
    const test_step = b.step("test", "Run unit tests");
    test_step.dependOn(&run_lib_unit_tests.step);
    test_step.dependOn(&run_exe_unit_tests.step);
}
