module moorse_code_blinker (
//module tester ( // use for DeSIM
    input CLOCK_50,
    input [9:0] SW,
    input [3:0] KEY,
    output [9:0] LEDR
);

  // wires
  wire resetn;
  wire start_signal;

  // Registers
  reg out_clk;
  reg [9:0] moorse_signal;
  reg [25:0] time_counter;
  reg [1:0] state, next_state;
  reg [3:0] LED_counter, msg_len;
  reg LED_output;

  // parameters
  parameter A = 10'b0000010111, B = 10'b0111010101;
  parameter Sidle = 2'b00, Sdisplay = 2'b01, Sshift = 2'b10;
  parameter count_value = 25000000;
  //parameter count_value = 250000; // use lower value for DeSIM

  // assign statements
  assign start_signal = ~KEY[0];  // start on press
  assign resetn = KEY[1];  // reset key
  assign LEDR[0] = LED_output;  // LED output



  // finite state machine
  always @(posedge CLOCK_50, negedge resetn) begin
    // state assignments
    if (~resetn) state = Sidle;
    else state = next_state;

    case (state)
      // only begin when pressed
      Sidle: begin
        next_state  <= start_signal ? Sdisplay : Sidle;
        LED_counter <= 0;  // reset signal
        LED_output  <= 0;
      end

      // display current state, change after 0.5 seconds
      Sdisplay: begin
        LED_output <= moorse_signal[LED_counter];
        next_state <= out_clk ? Sshift : Sdisplay;
      end

      // shift display, go back to display or reset if finished
      Sshift: begin
        LED_counter <= LED_counter + 1;
        next_state  <= (LED_counter == msg_len) ? Sidle : Sdisplay;
      end

      default: next_state = Sidle;

    endcase
  end



  // 0.5 second down counter
  always @(posedge CLOCK_50, negedge resetn) begin
    // reset timer on reset press
    if (~resetn) begin
      time_counter <= count_value;  // 0.5 seconds
      out_clk <= 0;

      // down counter, only count down when displaying
    end else if (state == Sdisplay) begin
      if (time_counter == 0) begin
        out_clk <= 1;
        time_counter <= count_value;
      end else begin
        time_counter <= time_counter - 1;
        out_clk <= 0;
      end

      // reset counter and out_clk signal
    end else begin
      out_clk <= 0;
      time_counter <= count_value;
    end
  end



  // Mux to determine signal
  always @(*) begin
    case (SW)
      10'd0: begin
        moorse_signal = A;
        msg_len = 5;
      end
      10'd1: begin
        moorse_signal = B;
        msg_len = 9;
      end

      default: begin
        moorse_signal = A;
        msg_len = 5;
      end
    endcase
  end

endmodule
