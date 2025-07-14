`ifndef HEXPATH
 `define HEXPATH ""
`endif

module ram_1r1w_async #(
    parameter int DataWidth        = 8,
    parameter int NumEntries       = 8,
    parameter string ReadmemhFilename = "memory_init_file.memh"
) (
    input  logic                          clk_i,
    input  logic                          reset_i,
    input  logic                          wr_valid_i,
    input  logic [DataWidth-1:0]         wr_data_i,
    input  logic [$clog2(NumEntries)-1:0] wr_addr_i,
    input  logic [$clog2(NumEntries)-1:0] rd_addr_i,
    output logic [DataWidth-1:0]         rd_data_o
);

  logic [DataWidth-1:0] ram [0:NumEntries-1];

  // initialize: always clear & load the ROM, but only dump in simulation
 initial begin
      $display("%m: NumEntries is %d, DataWidth is %d", NumEntries, DataWidth);
      $readmemh({`HEXPATH, ReadmemhFilename}, ram, 0, NumEntries-1);
      for (int i = 0; i < NumEntries; i++)
        $dumpvars(0, ram[i]);
   end

  assign rd_data_o = ram[rd_addr_i];

  always_ff @(posedge clk_i) begin
    if (wr_valid_i && !reset_i) begin
      ram[wr_addr_i] <= wr_data_i;
    end
  end

endmodule
