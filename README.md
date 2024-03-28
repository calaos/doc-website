# Documentation repo

This repo holds the doc for calaos. It is based on [Hugo](https://gohugo.io/) with the theme [Learn](https://learn.netlify.app)

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
