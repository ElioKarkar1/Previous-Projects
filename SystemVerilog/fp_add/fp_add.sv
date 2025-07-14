module fp_add import float_pkg::*; (
    input  logic   clk_i,
    input  logic   rst_ni,

    output logic   op_ready_o,
    input  logic   op_valid_i,
    input  float_t op_a_i,
    input  float_t op_b_i,

    input  logic   sum_ready_i,
    output logic   sum_valid_o,
    output float_t sum_data_o
);

// Utility: Counts leading zeros for normalization (used in normalization stage)
function automatic int count_leading_zeros(logic [MantissaWidth+4:0] in);
    for (int i = 0; i < $bits(in); i++) begin
        if (in[$bits(in)-1-i]) begin
            return i;
        end
    end
    return $bits(in);
endfunction

// STAGE: EXPONENT ALIGNMENT
// Calculate exponent difference and sticky mask for GRS
logic [BiasedExponentWidth-1:0] exponent_difference;
logic [MantissaWidth-1:0] op_sticky_mask;

always_comb begin
    op_sticky_mask = 0;
    if (op_a_q.biased_exponent < op_b_q.biased_exponent) begin
        exponent_difference = (op_b_q.biased_exponent - op_a_q.biased_exponent);
        if (exponent_difference > MantissaWidth+2)
            op_sticky_mask = '1;
        else if (exponent_difference > 2)
            op_sticky_mask = (1 << (exponent_difference-2)) - 1;
    end else begin
        exponent_difference = (op_a_q.biased_exponent - op_b_q.biased_exponent);
        if (exponent_difference > MantissaWidth+2)
            op_sticky_mask = '1;
        else if (exponent_difference > 2)
            op_sticky_mask = (1 << (exponent_difference-2)) - 1;
    end
end

wire [MantissaWidth:0] a_significand = float_significand(op_a_q);
wire [MantissaWidth:0] b_significand = float_significand(op_b_q);

logic [MantissaWidth+4:0] normalized_a_significand_w_grs;
logic [MantissaWidth+4:0] normalized_b_significand_w_grs;

// STAGE: SIGNIFICAND ALIGNMENT
// Shift significands to align exponents, insert GRS bits
always_comb begin
    normalized_a_significand_w_grs = {1'b0, a_significand, 3'b000};
    normalized_b_significand_w_grs = {1'b0, b_significand, 3'b000};
    if (op_a_q.biased_exponent < op_b_q.biased_exponent) begin
        normalized_a_significand_w_grs >>= exponent_difference;
        normalized_a_significand_w_grs[0] = |(op_a_q.mantissa & op_sticky_mask);
    end else begin
        normalized_b_significand_w_grs >>= exponent_difference;
        normalized_b_significand_w_grs[0] = |(op_b_q.mantissa & op_sticky_mask);
    end
end

// Pipeline: Latch aligned significands with GRS bits
logic [MantissaWidth+4:0] normalized_a_significand_w_grs_q;
logic [MantissaWidth+4:0] normalized_b_significand_w_grs_q;
always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
        normalized_a_significand_w_grs_q <= '0;
        normalized_b_significand_w_grs_q <= '0;
    end else if (state_q == ST_ALIGN) begin
        normalized_a_significand_w_grs_q <= normalized_a_significand_w_grs;
        normalized_b_significand_w_grs_q <= normalized_b_significand_w_grs;
    end
end

// STAGE: SIGNED ADDITION
// Form signed versions of significands, apply sign, and sum
logic signed [MantissaWidth+5:0] signed_normalized_a_significand_w_grs;
logic signed [MantissaWidth+5:0] signed_normalized_b_significand_w_grs;
always_comb begin
    signed_normalized_a_significand_w_grs = {1'b0, normalized_a_significand_w_grs_q};
    signed_normalized_b_significand_w_grs = {1'b0, normalized_b_significand_w_grs_q};
    if (op_a_q.sign) signed_normalized_a_significand_w_grs *= -1;
    if (op_b_q.sign) signed_normalized_b_significand_w_grs *= -1;
end

// Add the two signed significands
logic signed [MantissaWidth+5:0] signed_unnormalized_sum_w_grs;
logic [MantissaWidth+4:0]        unnormalized_sum_w_grs;
logic signed [MantissaWidth+5:0] signed_unnormalized_sum_w_grs_q;
logic [MantissaWidth+4:0]        unnormalized_sum_w_grs_q;
always_comb begin
    signed_unnormalized_sum_w_grs = signed_normalized_a_significand_w_grs + signed_normalized_b_significand_w_grs;
    if (signed_unnormalized_sum_w_grs[MantissaWidth+5])
        unnormalized_sum_w_grs = -signed_unnormalized_sum_w_grs[MantissaWidth+4:0];
    else
        unnormalized_sum_w_grs = signed_unnormalized_sum_w_grs[MantissaWidth+4:0];
end

// Pipeline: Latch sum for next stage
always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
        signed_unnormalized_sum_w_grs_q <= '0;
        unnormalized_sum_w_grs_q        <= '0;
    end else if (state_q == ST_ADD) begin
        signed_unnormalized_sum_w_grs_q <= signed_unnormalized_sum_w_grs;
        unnormalized_sum_w_grs_q        <= unnormalized_sum_w_grs;
    end
end

// STAGE: NORMALIZATION
// Normalize sum (shift left so top bit is set), track leading zeros for exponent adjust
wire int ffs_unnormalized_sum = count_leading_zeros(unnormalized_sum_w_grs_q);
wire [MantissaWidth+4:0] normalized_sum_grss = unnormalized_sum_w_grs_q << ffs_unnormalized_sum;

logic [MantissaWidth+4:0] normalized_sum_grss_q;
logic signed [MantissaWidth+5:0] signed_unnormalized_sum_w_grs_qq;
logic [MantissaWidth+4:0] unnormalized_sum_w_grs_qq;
logic [BiasedExponentWidth-1:0] exp_high_q, exp_low_q;
always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
        normalized_sum_grss_q <= '0;
        signed_unnormalized_sum_w_grs_qq <= '0;
        unnormalized_sum_w_grs_qq <= '0;
        exp_high_q <= '0;
        exp_low_q <= '0;
    end else if (state_q == ST_NORMALIZE) begin
        normalized_sum_grss_q <= normalized_sum_grss;
        signed_unnormalized_sum_w_grs_qq <= signed_unnormalized_sum_w_grs_q;
        unnormalized_sum_w_grs_qq <= unnormalized_sum_w_grs_q;
        exp_high_q <= op_a_q.biased_exponent > op_b_q.biased_exponent ? op_a_q.biased_exponent : op_b_q.biased_exponent;
        exp_low_q  <= op_a_q.biased_exponent > op_b_q.biased_exponent ? op_b_q.biased_exponent : op_a_q.biased_exponent;
    end
end

//STAGE: ROUNDING
// Apply GRS (Guard, Round, Sticky) bits and round, check if result overflows into new MSB
wire [MantissaWidth+3:0] normalized_sum_grs = (normalized_sum_grss_q >> 1) | normalized_sum_grss_q[0];

logic [MantissaWidth+1:0] big_normalized_sum;
logic exponent_change_on_round;
always_comb begin
    big_normalized_sum        = normalized_sum_grs >> 3;
    exponent_change_on_round  = 0;
    if (normalized_sum_grs[2]) begin
        if (normalized_sum_grs[1:0] != 2'b00) begin
            big_normalized_sum++;
        end else if (big_normalized_sum[0]) begin
            big_normalized_sum++;
        end
    end
    if (big_normalized_sum[MantissaWidth+1]) begin
        exponent_change_on_round = 1;
        big_normalized_sum       = big_normalized_sum >> 1;
    end
end

// Pipeline: Latch rounding result and exponent info
logic [MantissaWidth+1:0] big_normalized_sum_q;
logic exponent_change_on_round_q;
logic [BiasedExponentWidth-1:0] exp_high_qq;
logic [MantissaWidth+4:0] normalized_sum_grss_qq;
logic signed [MantissaWidth+5:0] signed_unnormalized_sum_w_grs_qqq;
logic [MantissaWidth+4:0] unnormalized_sum_w_grs_qqq;
always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
        big_normalized_sum_q <= '0;
        exponent_change_on_round_q <= '0;
        exp_high_qq <= '0;
        normalized_sum_grss_qq <= '0;
        signed_unnormalized_sum_w_grs_qqq <= '0;
        unnormalized_sum_w_grs_qqq <= '0;
    end else if (state_q == ST_ROUND) begin
        big_normalized_sum_q <= big_normalized_sum;
        exponent_change_on_round_q <= exponent_change_on_round;
        exp_high_qq <= exp_high_q;
        normalized_sum_grss_qq <= normalized_sum_grss_q;
        signed_unnormalized_sum_w_grs_qqq <= signed_unnormalized_sum_w_grs_qq;
        unnormalized_sum_w_grs_qqq <= unnormalized_sum_w_grs_qq;
    end
end

// STAGE: EXPONENT ADJUST
// Final exponent adjustment after rounding and normalization
logic [MantissaWidth:0] normalized_sum;
logic signed [BiasedExponentWidth-1:0] sum_biased_exponent;
always_comb begin
    normalized_sum = big_normalized_sum_q[MantissaWidth:0];
    sum_biased_exponent = exp_high_qq;
    sum_biased_exponent -= count_leading_zeros(unnormalized_sum_w_grs_qqq) - 1;
    if (exponent_change_on_round_q) sum_biased_exponent++;
    if (normalized_sum == 0) sum_biased_exponent = 0;
end

// Pipeline: Latch mantissa and exponent for output stage
logic [MantissaWidth:0] normalized_sum_q;
logic signed [BiasedExponentWidth-1:0] sum_biased_exponent_q;
always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
        normalized_sum_q      <= '0;
        sum_biased_exponent_q <= '0;
    end else if (state_q == ST_ADJUST_EXP) begin
        normalized_sum_q      <= normalized_sum;
        sum_biased_exponent_q <= sum_biased_exponent;
    end
end


float_t sum_data_working;
always_comb begin
    sum_data_working.sign            = signed_unnormalized_sum_w_grs_qqq[$bits(signed_unnormalized_sum_w_grs_qqq)-1];
    sum_data_working.mantissa        = normalized_sum_q[MantissaWidth-1:0];
    sum_data_working.biased_exponent = sum_biased_exponent_q;
end


typedef enum logic [2:0] { IDLE, ST_ALIGN, ST_ADD, ST_NORMALIZE, ST_ROUND, ST_ADJUST_EXP, ST_WRITE, DONE } state_t;
state_t state_d, state_q;
float_t op_a_d, op_a_q;
float_t op_b_d, op_b_q;
float_t sum_data_d, sum_data_q;

always_ff @(posedge clk_i) begin
    if (!rst_ni) begin
        state_q    <= IDLE;
        op_a_q     <= 'x;
        op_b_q     <= 'x;
        sum_data_q <= 'x;
    end else begin
        state_q    <= state_d;
        op_a_q     <= op_a_d;
        op_b_q     <= op_b_d;
        sum_data_q <= sum_data_d;
    end
end

always_comb begin
    state_d     = state_q;
    op_ready_o  = 0;
    sum_valid_o = 0;
    sum_data_d  = sum_data_q;
    op_a_d      = op_a_q;
    op_b_d      = op_b_q;
    sum_data_o  = 'x;

    if (state_q == IDLE) begin
        op_ready_o = 1;
        if (op_ready_o && op_valid_i) begin
            state_d = ST_ALIGN;
            op_a_d  = op_a_i;
            op_b_d  = op_b_i;
        end
    end else if (state_q == ST_ALIGN) begin    // Move to significand addition
        state_d = ST_ADD;
    end else if (state_q == ST_ADD) begin      // Move to normalization
        state_d = ST_NORMALIZE;
    end else if (state_q == ST_NORMALIZE) begin // Move to rounding
        state_d = ST_ROUND;
    end else if (state_q == ST_ROUND) begin    // Move to exponent adjustment
        state_d = ST_ADJUST_EXP;
    end else if (state_q == ST_ADJUST_EXP) begin // Move to write
        state_d = ST_WRITE;
    end else if (state_q == ST_WRITE) begin    // Latch result and move to done
        state_d    = DONE;
        sum_data_d = sum_data_working;
    end else if (state_q == DONE) begin        // Wait for output to be accepted, then restart
        sum_valid_o = 1;
        sum_data_o  = sum_data_q;
        if (sum_ready_i && sum_valid_o) begin
            state_d    = IDLE;
            sum_data_d = 'x;
            op_a_d     = 'x;
            op_b_d     = 'x;
        end
    end
end

endmodule
