# JSON Validator CLI

Crude & capable JSON validator command-line application. This is based on the code in [Chalet](https://github.com/chalet-org/chalet). It's designed to take any number of files, or a simple glob pattern -- a folder path with '*.json' at the end.

```
JSON Validator CLI
Usage:
   jsonv -s schema.json path/file1.json path/file2.json
   jsonv -s schema.json path/*.json
```

Define the schema with `-s [file]`
Right now, it only works with local files.