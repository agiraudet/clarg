# CLArg Library

CLArg is a command line argument parsing library written in C. It provides a simple and efficient way to manage command line arguments in your applications.

## Installation

To install the CLArg library for the whole system, follow these steps:

```bash
git clone https://github.com/agiraude/clarg.git
cd clarg
make
sudo make install
```

Otherwise, you can simply add the `clarg.h` and `clarg.c` files to your project and compile them with your application.

## Usage

To use the CLArg library in your C application, include the header file and use the provided functions to parse command line arguments.

```c
#include "clarg.h"

int main(int argc, char **argv) {
    t_cla cla;

    // Initialize the argument parser
    clarg_init(argc, argv);

    // Define your arguments here
    cla_arg(&cla, 'v', "version", "Print version information");
    cla_arg(&cla, 'o', "output", "Output file");

    // Parse the command line arguments
    clarg_parse();

    //Display the help message
    if (cla_provided(&cla, 'h'))
      cla_usage(&cla);

    // Use the parsed arguments
    printf("Output file: %s\n", cla_get(&cla, "output))

    cla_destroy(&cla);
    return 0;
}
```

## Example

Compile the example program using the following command:

```bash
make example
```

Use -h to display the automatically generated help message, or anything else to see the debug information:

```bash
Usage: ./example [OPTIONS]
Options:
  -h, --help                       Print this help message
  -v, --verbose                    Enable verbose mode
  -o <option>, --output=<option>   Specify the output file
  -e <option>, --encoding=<option> Specify the encoding
```

## API Reference

`t_cla *cla_init(t_cla *cla, int argc, char **argv)`

Initializes a command line argument parser with the given arguments.

`void cla_destroy(t_cla *cla)`

Destroys a command line argument parser, freeing any allocated resources.

`t_clarg *clarg_init(t_clarg *clarg, char short_name, const char *long_name, const char *description)`

Initializes a command line argument with a short name, long name, and description.

`t_clarg *cla_new(t_cla *cla)`

Creates a new command line argument parser.

`t_clarg *cla_arg(t_cla *cla, char short_name, const char *long_name, const char *description)`

Adds a new argument to a command line argument parser.

`t_clarg *clarg_add_allowed_value(t_clarg *clarg, const char *value)`

Adds an allowed value to a command line argument.

`int cla_parse(t_cla *cla)`

Parses the command line arguments using the given parser.

`void cla_usage(t_cla *cla)`

Prints the usage information for the command line arguments.

`bool cla_provided(t_cla *cla, char short_name)`

Checks if a command line argument was provided.

`t_clarg *cla_get(t_cla *cla, char short_name)`

Returns the command line argument with the given short name.

`const char *cla_value(t_cla *cla, char short_name)`

Returns the value of the command line argument with the given short name.

`void clarg_debug_print(t_clarg *clarg)`

Prints debug information for a command line argument.

`void cla_debug_print(t_cla *cla)`

Prints debug information for a command line argument parser.

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License

[MIT](https://choosealicense.com/licenses/mit/)

```

Please replace `https://github.com/yourusername/clarg.git` with the actual URL of your repository.
