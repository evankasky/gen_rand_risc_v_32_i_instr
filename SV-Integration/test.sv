module test;

    import "DPI" function int test_func();
    int x;

    initial begin
        x = test_func();
        $display(x);
    end

endmodule: test