package cli

type subcommand struct {
	Name         string
	Usage        string
	RequiredArgs int
	Flags        []flag
}

var registeredSubcommands []subcommand

func AddSubcommand(name string, usage string, reqArgs int) {
	registeredSubcommands = append(registeredSubcommands, subcommand{name, usage, reqArgs, nil})
}
