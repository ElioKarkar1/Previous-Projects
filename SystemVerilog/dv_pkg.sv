
package dv_pkg;

import float_pkg::*;

import "DPI-C" function real float2real(int in);
import "DPI-C" function int real2float(real in);

function automatic float_t rand_raw_float();
    real r;
    int sign;
    sign = $urandom_range(0,1) ? 1 : -1;
    if ($urandom_range(0,9) == 0) begin
        return '{ sign: sign, biased_exponent: '0, mantissa: '0 };
    end else begin
        r = ($urandom() % 2000000) / 1000.0 - 1000.0;
        return float_t'(real2float(r));
    end
endfunction

endpackage
