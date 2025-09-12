package main

import (
	"fmt"
	"os"
	"os/exec"
	"path/filepath"

	"github.com/SolarFlurry/Komodo/src/cmd/cli"
)

func exePath() string {
	var path, err = os.Executable()
	if err != nil {
		fmt.Println("exePath error: ", err)
		os.Exit(1)
	}
	return filepath.Dir(path)
}

func main() {
	// add subcommands
	cli.AddSubcommand("help", "Get help on the Komodo CLI")
	cli.AddSubcommand("build", "Compile a .kmd file")
	cli.AddSubcommand("version", "Display the Komodo version")

	cli.Parse()

	switch cli.Subcommand() {
	case "build":
		var file *os.File

		if cli.NArg() > 0 {
			var err error
			file, err = os.Open(cli.Arg(0) + ".kmd")
			if err != nil {
				fmt.Println(err)
			}
		} else {
			cli.Help()
			os.Exit(0)
		}

		transpilerPath := filepath.Join(exePath(), "kmd_compiler")
		var cmd = exec.Command(transpilerPath)
		cmd.Stdin = file

		var output, err = cmd.Output()
		fmt.Println(string(output))
		if err != nil {
			if exitError, ok := err.(*exec.ExitError); ok {
				fmt.Printf("transpiler failed: %d\n", exitError.ExitCode())
			} else {
				fmt.Printf("ERROR: %v\n", err)
			}
			os.Exit(1)
		}
	case "version":
		fmt.Println("Komodo v0.1-beta")
	default:
		cli.Help()
	}
}
