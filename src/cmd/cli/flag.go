package cli

import (
	"fmt"
)

type flag struct {
	Name  string
	Usage string
}

func AddFlag(subcmd string, name string, usage string) {
	var foundSubcmd subcommand
	var cmdIdx int
	for i, compSubcmd := range registeredSubcommands {
		if subcmd == compSubcmd.Name {
			foundSubcmd = compSubcmd
			cmdIdx = i
			break
		}
	}
	if foundSubcmd.Name == "" {
		fmt.Println("Unknown subcommand")
		return
	}
	foundSubcmd.Flags = append(foundSubcmd.Flags, flag{name, usage})
	registeredSubcommands[cmdIdx] = foundSubcmd
}
