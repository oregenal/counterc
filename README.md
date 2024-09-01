# Counter

Simple countcer, that read keyboard device & on + increase countcer in a file

## Seteuid example

Copy to PATH dir
```console
cp countc /usr/local/bin
```

Set owner group `input`
```console
chown root:input countc
```

Set set-bit for group
```console
chmod g+s countc
```
