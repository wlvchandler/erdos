### Channels
In imperative languages, statements are separated by something like `;` or `EOL`.  
In Erdos, statements are separated by `->`.  

However, in C-like languages `;` does nothing other than terminate the statement.

Channels are native operators used to channel/direct data.
The nodes they connect are its _endpoints_: _source_ and _destination_ .

The runtime treats channels as suspension points. That is, they are why Erdos is capable of large scale native natural concurrency. Nodes will not (fully) execute until their input channel signals, but unless specified otherwise, the runtime will attempt to pre/partial execute as many nodes in a stream as possible.

|  channel  | operator |  
| :--------: | :--------: |
| input     | `<~`       | 
| output    | `~>`       | 
| auxiliary | `<-`       |
| flow      | `->`       | 
| condition | `?>`       |
| positive  | `+>`       | 
| negative  | `/>`       | 

### data lists
Basic syntax of a data list is the list of types within [] followed by the data names, and default values (optional):

<table>
<tr>
<td><strong>Erdos</strong></td>
<td><strong>C Like</strong></td>
</tr>
<tr>
<td>
<pre>
1 $Number x -> 2 $Number y -> 3 $Number z 
</pre>
</td>
<td>
<pre>
int x = 1, int y = 2, int z = 3
</pre>
</td>
</tr>
</table>


### Nodes
With the exception of channels, everything in Erdos is a node.

A **node** is a computation unit.

A **flow** is a series of nodes and channels. 



```
<node_name>
  <~ [<type0> ...] <fieldName0> ... // inputs
  ~> [<type0> ...] <fieldName0> ... // outputs
  <- [<type0> ...] <fieldName0> ... // extra data - not input
  -> <flow>
```

So there are 3 data channels:
    Inputs:  what the node can accept
    Outputs: what the node can send
    Extras:  internal pre-defined data (i.e. local variables)

And a flow channel:

|  type  | prefix  | 
| :----: | :-----: | 
| basic  | none |
| stream | `*` or surrounded by `()` |
| event  | `+` or surrounded by `[]` |

Node Types:
```
type   | prefix 
-------+-------------------------
basic  | none
stream | `*` or surrounded by ()
event  | `+` or surrounded by []
```

Basic Nodes: are similar to functions and variables

```
Square 
    <~ [number] x 
    ~> [^] 
    -> x * 2 
```

Can write it all on one line too:
```
Square <~ [number] x ~> [^] -> x * 2 
```


So with 
struct MyStruct {
    number x;
    number y;
}
```
MyStruct <~ [number number] x y 
```

Since it's still a node, we can have logic too:
```
MyStruct <~ [number number] x y -> x * y
```


Examples:

Simplest node is:
```
foo -> . 
```
where `.` is a no-op


*Main -> 1 2 MyNode -> "Event ID:" " " .! Join -> Print
(Main) -> 1 2 MyNode -> "Event ID:" " " .! Join -> Print
