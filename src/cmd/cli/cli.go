package cli

import (
	"fmt"
	"os"
	"slices"
)

func Help() {
	fmt.Println("\nkomodo <command> <flags>")
	fmt.Print("\nUsage:\n\n")
	for _, subcmd := range registeredSubcommands {
		fmt.Printf("%s\t\t\t%s\n", subcmd.Name, subcmd.Usage)
		for _, flag := range subcmd.Flags {
			fmt.Printf("\t-%s\t%s\n", flag.Name, flag.Usage)
		}
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
var currentSubcommand subcommand

// Parse the arguments
func Parse() {
	if len(os.Args) > 1 {
		var startCheck = 1
		// check if subcommand
		for _, subc := range registeredSubcommands {
			if subc.Name == os.Args[1] {
				currentSubcommand = subc
				startCheck = 2
				break
			}
		}
		// check for flags
		for _, arg := range os.Args[startCheck:] {
			if arg[0] == '-' {
				var isFlag bool = false
				for _, flag := range currentSubcommand.Flags {
					if arg[1:] == flag.Name {
						flags = append(flags, arg[1:])
						isFlag = true
						break
					}
				}
				if !isFlag {
					fmt.Println("\x1b[33mWARNING: Flag '", arg, "' does not exist.\x1b[0m")
				}
			} else {
				args = append(args, arg)
			}
		}
		if len(args) < currentSubcommand.RequiredArgs {
			fmt.Println("\x1b[31mError: Insufficient arguments\x1b[0m")
			os.Exit(0)
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
	return currentSubcommand.Name
}
