# cloak 

## Usage

### Encrypt a file 
```bash
cloak encrypt -i <PATH> [-o <PATH>]
```
> Default output: `<FILE>.cloak` 

### Decrypt a file
```bash
cloak decrypt -i <PATH> -o <PATH> -k <KEY-PATH>
```

### Get the hash of a file
```bash
cloak hash -i <PATH>
```

## Ignore file

The `.cloakignore` file is being used to define the patterns that are being ignored in the encryption process.

Anything inside the `.gitignore` file is also included in this list

### Patterns ignored by default

- `.git*`
- `.cloakignore`
- `*.cloak`
- `cloak.ini`

## License

[MIT](./LICENSE)
