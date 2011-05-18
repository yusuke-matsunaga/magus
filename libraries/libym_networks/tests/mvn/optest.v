module vtest(input [7:0] a, b,
             output x, y, z,
             output w, u,
             output [7:0] v,
             output [7:0] t);
  wire [7:0] a;
  wire [7:0] b;

  wire x = &a;
  wire y = |a;
  wire z = ^a;

  wire w = a == b;
  wire u = a < b;
  wire [7:0] v = a + b;
  
  wire [7:0] t = - a;

endmodule

