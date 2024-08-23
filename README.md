# Counter

Simple counter, that read keyboard device & on + increase counter in a file

## Seteuid example

Copy to PATH dir
```console
cp dcount /usr/local/bin
```

Set owner.
```console
chown root:input dcount
```

Set stiky bit for group `input`
```console
chmod g+s dcount
```
