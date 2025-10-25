package main

import (
	"fmt"
	"os"
	"os/exec"
	"path/filepath"
	"strings"

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
	cli.AddSubcommand("help", "Get help on the Komodo CLI", 0)
	cli.AddSubcommand("build", "Compile a .kmd file", 1)
	cli.AddSubcommand("version", "Display the Komodo version", 0)
	// add flags
	cli.AddFlag("build", "o", "Controls the output of the compilation")

	cli.Parse()

	switch cli.Subcommand() {
	case "build":
		var file *os.File

		if cli.NArg() > 0 {
			var err error
			var filepath = cli.Arg(0)
			if !strings.HasSuffix(filepath, ".kmd") {
				filepath += ".kmd"
			}
			file, err = os.Open(filepath)
			if err != nil {
				cli.Error("File '" + filepath + "' does not exist")
				os.Exit(0)
			}
		} else {
			os.Exit(0)
		}

		transpilerPath := filepath.Join(exePath(), "kmd_compiler")
		var cmd = exec.Command(transpilerPath)
		cmd.Stdin = file

		cmd.Stdout = os.Stdout
		cmd.Stderr = os.Stderr

		var err = cmd.Run()
		if err != nil {
			if exitError, ok := err.(*exec.ExitError); ok {
				fmt.Printf("transpiler failed: %d\n", exitError.ExitCode())
			} else {
				fmt.Printf("Error: %v\n", err)
			}
			os.Exit(1)
		}
	case "help":
		cli.Help()
	case "version":
		fmt.Println("Komodo version 0.1-beta")
	default:
		fmt.Println("Use 'komodo help' to get help on Komodo")
	}
}
