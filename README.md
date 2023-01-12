# Password Manager

- Store login credentials (email, pseudo, passwords, url, notes ...) in a secure way.
- Generate passwords fully customizable (length, characters, etc.).
- Import and export data in csv format.
- Encrypting stored data.
- Bonus: If we have time, make a graphical interface with GTK.

## Workflow

### Issues

- **Include it in [kanban board](#kanban-board) (see _Projects_ in sidebar)**
- Just name your issue with few words. Example: `Add user authentication`
- **Use labels** to categorize your issue. Example: `feature`, `enhancement`,
  `bug`, `question`, ...
- **Don't assign someone right away**. We'll assign someone when we'll discuss

### Branches

- `main`: production branch
- `dev`: development branch
- `#-issue-name`: feature/fix branches where `#` is the issue number. Those
  branches can be directly created from the issue page (see _Development_
  in sidebar)

### Pull requests

- Copy the name of the issue you're working on
- **Don't merge your own pull request**
- **Don't merge pull requests that are not ready**
- **Don't put pull requests in the kanban project**, it will duplicate the
  issue
- **Squash and merge** pull requests. This will merge all commits into one
  (cleaner)
- **Delete the branch** after merging

### Kanban board

The kanban board is available [here](https://github.com/users/opixelum/projects/9/).

Statuses:

- **Discussion**: issues to discuss about, where we assign tasks
- **To do**: assigned tasks to do
- **In progress**: tasks currently being worked on
- **In review**: changes that are ready for review or that are being reviewed
- **Ready for dev**: changes approved, ready to be merged to `dev` branch
- **Ready for production**: features/fixes merged in `dev` branch, tested &
  ready to be merged to `main` branch
- **Done**: Features/fixes merged to `main` branch

## Contributing

### Rules

- **Never commit directly to `main` branch**
- **Feature branches must be merged to `dev` branch before merging to `main`**
- When creating an issue, **use labels**
- Always create a **new branch** for **your changes**
- Always create a **pull request** for **your changes**
- Follow the **[coding style](#coding-style)**

## Coding style

### Naming

- `snake_case` for variables
- `camelCase` for functions
- `PascalCase` for structures and enums
- `UPPERCASE` for constants

### Comments

- `//` for single line comments
- `/* */` for multi line comments
- `/** */` for documentation comments

Example:

```c
    // This is a single line comment

    /*
     * This is a multi line comment
     */

    /**
     * @brief This is a documentation comment
     *
     * @param a This is a parameter
     * @param b This is another parameter
     *
     * @return The sum of a and b
     */
    int foo (int a, int b)
    {
        return a + b;
    }
```

### Indentation

- 4 spaces for indentation
- Don't use tabs

### Line length

- 80 characters
- In some rare cases, it's okay to go over 80 characters

### Braces

- Braces are not required for single line blocks
- Braces have its own line

Example:

```c
    main (int argc, char **argv)
    {
        // ...
    }
```

### Parameters

- For function with a lot of parameters, put each parameter on a new line
- In this case, the parenthesis are on its own line

Example:

```c
    void foo
    (
        int a,
        int b,
        int c
    )
    {
        // ...
    }
```
