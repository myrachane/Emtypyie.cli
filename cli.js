#!/usr/bin/env node

const readline = require('readline');
const chalk = require('chalk');
const getCommand = require('./commands/get');
const helpCommand = require('./commands/help');

const BANNER = `
  ███████╗███╗   ███╗████████╗██╗   ██╗██████╗ ██╗   ██╗██╗███████╗
  ██╔════╝████╗ ████║╚══██╔══╝╚██╗ ██╔╝██╔══██╗╚██╗ ██╔╝██║██╔════╝
  █████╗  ██╔████╔██║   ██║    ╚████╔╝ ██████╔╝ ╚████╔╝ ██║█████╗  
  ██╔══╝  ██║╚██╔╝██║   ██║     ╚██╔╝  ██╔═══╝   ╚██╔╝  ██║██╔══╝  
  ███████╗██║ ╚═╝ ██║   ██║      ██║   ██║        ██║   ██║███████╗
  ╚══════╝╚═╝     ╚═╝   ╚═╝      ╚═╝   ╚═╝        ╚═╝   ╚═╝╚══════╝
`;

const retro = chalk.hex('#33ff33');
const retroDim = chalk.hex('#1a7a1a');
const retroAccent = chalk.hex('#66ff66');

const projects = {
  qrkraft: require('./projects/qrkraft')
};

async function interactive() {
  const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    prompt: retro('>> ')
  });

  console.clear();
  console.log(retro(BANNER));
  console.log(retroDim('  ─────────────────────────────────────────────'));
  console.log(retro('  "code. create. conquer."'));
  console.log(retroDim('  ─────────────────────────────────────────────'));
  console.log();
  console.log(retroDim('  ├ ') + retroAccent('get <project>') + retroDim(' ─ install a project'));
  console.log(retroDim('  ├ ') + retroAccent('help') + retroDim('        ─ show available commands'));
  console.log(retroDim('  ├ ') + retroAccent('exit') + retroDim('        ─ quit'));
  console.log(retroDim('  └────────────────────────────────────────────'));
  console.log();

  rl.prompt();

  rl.on('line', async (line) => {
    const input = line.trim();
    const parts = input.split(/\s+/);
    const cmd = parts[0]?.toLowerCase();
    const arg = parts.slice(1).join(' ');

    if (!cmd || cmd === '') {
      rl.prompt();
      return;
    }

    if (cmd === 'exit' || cmd === 'quit') {
      console.log(retroDim('\n  System halted.'));
      rl.close();
      process.exit(0);
      return;
    }

    if (cmd === 'help') {
      helpCommand.show(projects);
    } else if (cmd === 'get') {
      if (!arg) {
        console.log(chalk.red('  Specify a project name. Usage: get <project>'));
        console.log(retroDim('  Available: ') + retro(Object.keys(projects).join(', ')));
      } else if (projects[arg.toLowerCase()]) {
        await getCommand.install(arg.toLowerCase(), projects[arg.toLowerCase()]);
      } else {
        console.log(chalk.red(`  Unknown project "${arg}".`));
        console.log(retroDim('  Available: ') + retro(Object.keys(projects).join(', ')));
      }
    } else {
      console.log(chalk.red(`  Unknown command "${cmd}". Type help for available commands.`));
    }

    rl.prompt();
  });

  rl.on('close', () => {
    console.log(retroDim('\n  System halted.'));
    process.exit(0);
  });
}

async function direct(args) {
  const cmd = args[0]?.toLowerCase();

  if (cmd === 'help' || !cmd) {
    helpCommand.show(projects);
    return;
  }

  if (cmd === 'get') {
    const projectName = args[1]?.toLowerCase();
    if (!projectName) {
      console.log(chalk.red('Specify a project name. Usage: emtypyie get <project>'));
      process.exit(1);
    }
    if (!projects[projectName]) {
      console.log(chalk.red(`Unknown project "${projectName}".`));
      process.exit(1);
    }
    await getCommand.install(projectName, projects[projectName]);
    return;
  }

  console.log(chalk.red(`Unknown command "${cmd}".`));
  process.exit(1);
}

const args = process.argv.slice(2);

if (args.length === 0) {
  interactive();
} else {
  direct(args);
}
