module compare;

   reg [7:0] a;
   reg [7:0] b;
   reg 	     go_a;
   reg 	     go_b;
   reg 	     error;

   wire      x1, x2;
   wire      y1, y2;
   wire      z1, z2;
   wire      w1, w2;
   wire      u1, u2;
   wire [7:0] v1, v2;
   wire [7:0] t1, t2;

   vtest vtest1(.a(a), .b(b), .x(x1), .y(y1), .z(z1),
	    .w(w1), .u(u1), .v(v1), .t(t1));
   vtest2 vtest2(.a(a), .b(b), .x(x2), .y(y2), .z(z2),
	     .w(w2), .u(u2), .v(v2), .t(t2));


   initial
     begin
	error = 0;
	go_a = 1;
	a = 8'b00000000;
	while ( go_a ) begin : loop
	   go_b = 1;
	   b = 8'b00000000;
	   while ( go_b ) begin

	      #5;
	      if ( x1 != x2 ) begin
		 $display("a = %h, b = %h: x1 = %h, x2 = %h",
			  a, b, x1, x2);
		 error = 1;
	      end
	      if ( y1 != y2 ) begin
		 $display("a = %h, b = %h: x1 = %h, x2 = %h",
			  a, b, y1, y2);
		 error = 1;
	      end
	      if ( z1 != z2 ) begin
		 $display("a = %h, b = %h: x1 = %h, x2 = %h",
			  a, b, z1, z2);
		 error = 1;
	      end
	      if ( w1 != w2 ) begin
		 $display("a = %h, b = %h: x1 = %h, x2 = %h",
			  a, b, w1, w2);
		 error = 1;
	      end
	      if ( u1 != u2 ) begin
		 $display("a = %h, b = %h: x1 = %h, x2 = %h",
			  a, b, u1, u2);
		 error = 1;
	      end
	      if ( v1 != v2 ) begin
		 $display("a = %h, b = %h: x1 = %h, x2 = %h",
			  a, b, v1, v2);
		 error = 1;
	      end
	      if ( t1 != t2 ) begin
		 $display("a = %h, b = %h: x1 = %h, x2 = %h",
			  a, b, t1, t2);
		 error = 1;
	      end
	      if ( error ) disable loop;

	      b = b + 8'b00000001;
	      if ( b == 8'b00000000 ) go_b = 0;
	   end
	   a = a + 8'b00000001;
	   if ( a == 8'b00000000 ) go_a = 0;
	end
	$finish;
     end

   /*
   initial
     $monitor ($stime, ": a = %h, b = %h: x = (%h, %h), y = (%h, %h), z = (%h, %h), w = (%h, %h), u = (%h, %h), v = (%h, %h), t = (%h, %h)",
	       a, b, x1, x2, y1, y2, z1, z2, w1, w2, u1, u2, v1, v2, t1, t2);
    */


endmodule