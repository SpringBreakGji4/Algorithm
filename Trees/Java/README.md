# Java Trie / Prefix Tree Demo

## Build

```bash
cd Java
javac -d bin src/TrieDemo.java
```

## Run

```bash
cd Java
java -cp bin TrieDemo
```

## Run with a custom query file

```bash
cd Java
java -cp bin TrieDemo ../test_case/trie_queries.txt
```

## Supported commands

- `insert <word>`
- `search <word>`
- `prefix <prefix>`
- `delete <word>`
- `autocomplete <prefix>`
- `countprefix <prefix>`
