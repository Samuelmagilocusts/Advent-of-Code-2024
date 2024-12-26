const std = @import("std");
const math = std.math;
const allocator = std.heap.page_allocator;

pub fn get_next(sn: u64) !u64 {
    const step1 = sn * 64;
    const step2 = sn ^ step1;
    const step3 = step2 % 16777216;
    const step4 = step3 / 32;
    const step5 = step4 ^ step3;
    const step6 = step5 % 16777216;
    const step7 = step6 * 2048;
    const step8 = step7 ^ step6;
    return step8 % 16777216;
}

pub fn main() !void {
    var file = try std.fs.cwd().openFile("input.txt", .{});
    defer file.close();

    var buf_reader = std.io.bufferedReader(file.reader());
    var in_stream = buf_reader.reader();

    var buf: [20]u8 = undefined;
    var input_sn = std.ArrayList(u64).init(allocator);

    while (try in_stream.readUntilDelimiterOrEof(&buf, '\n')) |line| {
        var num_str: [20]u8 = undefined;
        var num_str_len: usize = 0;

        for (line) |byte| {
            if (byte >= '0' and byte <= '9') {
                num_str[num_str_len] = byte;
                num_str_len += 1;
            } else {
                break;
            }
        }

        const num = try std.fmt.parseInt(u64, num_str[0..num_str_len], 10);
        try input_sn.append(num);
    }

    var total: u64 = 0;
    var total_p2: i64 = 0;
    var last_total: u64 = input_sn.items[0];
    var totals = std.AutoHashMap([4]i64, i64).init(allocator);
    const start = std.time.milliTimestamp();

    for (input_sn.items) |num| {
        // std.debug.print("new {}\n", .{num});
        var sub_tol = num;
        var totals_discrepancy = std.ArrayList(i64).init(allocator);
        var value_last_char = std.ArrayList(i64).init(allocator);
        for (0..2000) |_| {
            sub_tol = try get_next(sub_tol);
            try totals_discrepancy.append(@as(i64, @intCast(sub_tol % 10)) - @as(i64, @intCast(last_total % 10)));
            try value_last_char.append(@as(i64, @intCast(sub_tol % 10)));
            last_total = sub_tol;
        }

        var values_map = std.AutoHashMap([4]i64, i64).init(allocator);
        for (0..totals_discrepancy.items.len) |i| {
            if (i >= 3) {
                const pat: [4]i64 = [_]i64{ totals_discrepancy.items[i - 3], totals_discrepancy.items[i - 2], totals_discrepancy.items[i - 1], totals_discrepancy.items[i] };
                if (!values_map.contains(pat)) {
                    try values_map.put(pat, value_last_char.items[i]);
                }
            }
        }

        var it = values_map.iterator();
        while (it.next()) |kv| {
            const value: i64 = totals.get(kv.key_ptr.*) orelse 0;
            // std.debug.print("Totals: {any}:{}\n", .{ kv.key_ptr.*, kv.value_ptr.* });
            if (value != 0) {
                try totals.put(kv.key_ptr.*, value + kv.value_ptr.*);
            } else {
                try totals.put(kv.key_ptr.*, kv.value_ptr.*);
            }
        }

        total += sub_tol;
        // const end = std.time.milliTimestamp();
        // std.debug.print("Time elapsed: {} milliseconds. num: {}\n", .{ end - start, num });
    }

    var it = totals.iterator();
    while (it.next()) |kv| {
        // std.debug.print("Totals: {any}:{}\n", .{ kv.key_ptr.*, kv.value_ptr.* });
        if (total_p2 < kv.value_ptr.*) {
            total_p2 = kv.value_ptr.*;
        }
    }

    std.debug.print("AOC Day 22 Part 1 Total: {}\n", .{total});
    std.debug.print("AOC Day 22 Part 2 Total: {}\n", .{total_p2});
    const end = std.time.milliTimestamp();
    std.debug.print("Time elapsed: {} milliseconds.\n", .{end - start});
}
