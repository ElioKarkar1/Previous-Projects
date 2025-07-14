
module fifo_1r1w #(
    /* verilator lint_off WIDTHTRUNC */
    parameter int DataWidth = 8,
    // Note: Not NumEntries! NumEntries should be 1<<NumEntriesLog2
    parameter int NumEntriesLog2 = 8
    /* verilator lint_on WIDTHTRUNC */
) (
    input  logic clk_i,
    input  logic reset_i,

    input  logic [DataWidth-1:0] data_i,
    input  logic                 valid_i,
    output logic                 ready_o,

    output logic                 valid_o,
    output logic [DataWidth-1:0] data_o,
    input  logic                 ready_i
);

    localparam logic [NumEntriesLog2:0] Depth = 1 << NumEntriesLog2;
    logic [DataWidth-1:0] mem [0:Depth-1];

    logic [NumEntriesLog2-1:0] wr_ptr, rd_ptr;
    logic [NumEntriesLog2:0]   count, count_n;

    assign ready_o = (count != Depth);
    assign valid_o = (count != 0);

    always_comb begin
        case ({valid_i & ready_o, valid_o & ready_i})
            2'b10: count_n = count + 1;
            2'b01: count_n = count - 1;
            default: count_n = count;
        endcase
    end

    always_ff @(posedge clk_i) begin
        if (reset_i) begin
            wr_ptr <= '0;
            rd_ptr <= '0;
            count  <= '0;
        end else begin
            count <= count_n;
            if (valid_i & ready_o)
                wr_ptr <= wr_ptr + 1;
            if (valid_o & ready_i)
                rd_ptr <= rd_ptr + 1;
        end
    end

    always_ff @(posedge clk_i) begin
        if (valid_i & ready_o)
            mem[wr_ptr] <= data_i;
    end

    assign data_o = mem[rd_ptr];

endmodule
