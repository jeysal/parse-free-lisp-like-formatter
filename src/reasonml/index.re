let format = code => code |> Lexer.tokenize |> Printer.print;
