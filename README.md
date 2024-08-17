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
    // Initialize the argument parser
    clarg_init(argc, argv);

    // Define your arguments here
    cla_arg('v', "version", "Print version information");
    cla_arg('o', "output", "Output file");

    // Parse the command line arguments
    if (clarg_parse() <= 0)
      return 1;

    //Display the help message
    if (cla_provided('h')) {
      cla_usage();
      return 0;
    }

    // Use the parsed arguments
    printf("Output file: %s\n", cla_get('o'));

    return 0;
}
```

## Example

Compile the example program using the following command:

```bash
make example
```

Use -h to display the automatically generated help message, or -d to see the debug information:

```bash
This is a simple example program
Usage: ./example [OPTIONS] SOURCE 
Options:
  -h, --help                       Print this help message
  -d, --debug                      Show debug information
  -v, --verbose                    Enable verbose mode
  -o <option>, --output=<option>   Specify the output file
  -e <option>, --encoding=<option> Specify the encoding
```

## API Reference

`t_cla *cla_init(int argc, char **argv)`

Initializes a command line argument parser with the given arguments.
Should be called before defining any arguments.

`int cla_add_required_input(const char *input_name);`

Add a required input not related to a specific argument.

`void cla_add_description(const char *description);`

Add a description for the program.

`t_clarg *cla_arg(char short_name, const char *long_name, const char *description)`

Adds a new argument to a command line argument parser.
Short names are mandatory and must be unique (NULL will be returned if a duplicate is found).

`t_clarg *clarg_add_allowed_value(t_clarg *clarg, const char *value)`

Adds an allowed value to a command line argument.
If no allowed values are defined, any value is accepted.

`int cla_parse()`

Parses the command line arguments and returns 0 if successful, -1 if an error occurred, or 1 if -h was provided.

`void cla_usage()`

Prints the usage information for the command line arguments.

`bool cla_provided(char short_name)`

Checks if a command line argument was provided.

`t_clarg *cla_get(char short_name)`

Returns the command line argument with the given short name.

`const char *cla_value(char short_name)`

Returns the value of the command line argument with the given short name.

`void clarg_debug_print(t_clarg *clarg)`

Prints debug information for a command line argument.

`void cla_debug_print()`

Prints debug information for all command line arguments.

## Contributing

Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License

[MIT](https://choosealicense.com/licenses/mit/)
