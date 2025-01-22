## vure, a simple interpreter
```
# this is a comment
builtin proc print(str text) void;

proc main(int st) void {
	str username = "lumix";
	print("hello", username);

  int num = 7;
  print(num, "->", square(num));
}

proc square(int x) int {
  return x * x;
}
```
