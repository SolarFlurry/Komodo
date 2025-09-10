package cli

import (
	"fmt"
	"os"
	"slices"
)

var registeredSubcommands []subcommand

func AddSubcommand(name string, usage string) {
	registeredSubcommands = append(registeredSubcommands, subcommand{name, usage})
}

func Help() {
	fmt.Println("\nKomodo usage:")
	fmt.Println("\nSubcommands:")
	for _, subcmd := range registeredSubcommands {
		fmt.Printf("\t%s\t\t%s\n", subcmd.Name, subcmd.Usage)
	}
	fmt.Println("")
}

/* ---------------------------- *\
 * VALUES GENERATED VIA PARSING *
\* ---------------------------- */
// list of flags
var flags []string

// list of arguments
var args []string

// subcommand used
var currentSubcommand string

// Parse the arguments
func Parse() {
	if len(os.Args) > 1 {
		var startCheck = 1
		// check if subcommand
		for _, subc := range registeredSubcommands {
			if subc.Name == os.Args[1] {
				currentSubcommand = os.Args[1]
				startCheck = 2
				break
			}
		}
		for _, arg := range os.Args[startCheck:] {
			if arg[0] == '-' {
				flags = append(flags, arg[1:])
			} else {
				args = append(args, arg)
			}
		}
	}
}

// Returns all arguments
func Args() []string {
	return args
}

// Returns th argument at index
func Arg(index int) string {
	return args[index]
}

// Returns the number of non-flag arguments
func NArg() int {
	return len(args)
}

// Returns all flags
func Flags() []string {
	return flags
}

// Returns whether flags contains 'flag'
func HasFlag(flag string) bool {
	return slices.Contains(flags, flag)
}

// Returns the subcommand used, "" if none
func Subcommand() string {
	return currentSubcommand
}
