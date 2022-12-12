# Jacob's Strategy Combiner

An application for optimizing the combination of Jacob L's strategies.

- Reads input files containing strategy results.  e.g.

```
Strategy B
1/2/2022,$ 98.66
1/3/2022,$ 256.66
1/4/2022,$(203.55)
1/5/2022,$ (56.36)
1/5/2022,$ 365.25
```

- Files can be selected one by one using `File | Open` menu item.
- File groups can be selected by dragging and dropping from file explorer.
- Parses the files and combines results from a single day
- Combines strategies
- Ranks strategies, original and combined, by total profit divided by maximum day loss if any
- Displays stategies, original and combined, in descending rank order
- Start fresh with `File new` menu item

![image](https://user-images.githubusercontent.com/2046227/207142083-3d356f64-73ae-4ef9-81ad-3890a447e4bc.png)
