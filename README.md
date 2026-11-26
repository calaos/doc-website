# Documentation repo

This repo holds the doc for calaos. It is based on [Hugo](https://gohugo.io/) with the theme [Relearn](https://mcshelby.github.io/hugo-theme-relearn/)

## Usage

The easiest way to contribute is to use Visual Studio Code and open this repo. You will need a running Docker installation to let VSCode use the existing `devcontainer` (Click on Reopen in container).

There is also a Task: F1 > `Tasks: Run Task` > `hugo` > `hugo: Serve site`. You can then open a local running doc website [http://localhost:1313](http://localhost:1313) that is refreshed automatically when you edit pages.

## New content

To create new content, you need to use the existing templates. There are 2 types:
 
 - Chapters: Used for pages that are chapters title
 - Normal pages: Normal pages for content

In a terminal inside VSCode:
```
hugo new --kind chapter <subfolder>/_index.fr.md
hugo new --kind chapter <subfolder>/_index.en.md
```

```
hugo new <subfolder>/some-content.fr.md
hugo new <subfolder>/some-content.en.md
```

> Don't forget to create both language files

## Build

- Locally: the devcontainer (`.devcontainer/devcontainer.json`) installs Hugo extended through the `hugo` devcontainer feature; nothing is installed on the host. Preview with `hugo server`.
- CI (`.github/workflows/build.yml`) uses `peaceiris/actions-hugo` with Hugo `latest`, extended, and runs `hugo --minify`. The site must therefore build with the newest Hugo, not a pinned one.
- The `Relearn` theme 9.5.2 is vendored in `themes/hugo-theme-relearn/`, not a submodule: to update it, replace the folder with a release archive from [GitHub](https://github.com/McShelby/hugo-theme-relearn/releases) and drop its `go.mod`. It needs Hugo 0.166.0 or newer. Site tweaks live outside the theme: the `calaos` variant (`assets/css/theme-calaos.css`), `layouts/partials/` and `i18n/`. Mermaid is the version shipped with the theme.

## Tools

- `tools/screenshots/`: regenerates the climate screenshots of `content/calaos_installer/devices/images/` from one invented house (web app, mobile, remote UI, installer); see its README for each command and the commit it was last run against.
- `tools/examples/`: checks the example projects of `static/examples/climat/` (calaos_server loads them, the installer opens and re-saves them unchanged) and `zip.py --check` fails when a zip no longer matches its folder; run `zip.py` after editing an example.
- Cross-repo notes: [calaos_base docs/18_stack_notes.md](https://github.com/calaos/calaos_base/blob/master/docs/18_stack_notes.md) (in French).
