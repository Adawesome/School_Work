// Learn more about F# at http://fsharp.org
// See the 'F# Tutorial' project for more help.
type Point = { X: float; Y: float; }
type HitResult =
    | Hit
    | Miss of float
let makePoint x y =
    {X = x; Y = y;}
type System.Random with
    /// Generates an infinite sequence of random numbers within the given range.
    member this.GetValues(minValue, maxValue) =
        Seq.initInfinite (fun _ -> this.Next(minValue, maxValue))

let r = System.Random()
let randomPoint (ran:System.Random) =
    let x = r.NextDouble()
    let y = r.NextDouble()
    makePoint x y
//recursively con to a list of randomPoints
let throwDarts n =
    List.init n (fun index -> randomPoint r)

let listLength coll =
     coll |> List.length |> printfn "List of list is: %d"

let isHit (point:Point) = 
    let circleBarrier = 
        (pown point.X 2) + (pown point.Y 2)
    //printfn "The value of this point is: %A" circleBarrier
    if circleBarrier <= 1.0 then
        HitResult.Hit
    else
        HitResult.Miss (circleBarrier - 1.0)

//HitResult list -> int
let rec helper coll value =
    if List.isEmpty coll then
        value
    else
        let front = 
            coll
            |> List.head
        match front with
        | Hit ->
            helper (coll |> List.tail) (value + 1)
        | _ ->
            helper (coll |> List.tail) (value)
    
//    match coll with
//    | []  -> value
//    | [Hit] ->
//        printfn "making recursion: %d" value
//        helper (coll |> List.tail ) (value + 1.0)
//    | _ ->
//            helper (coll |> List.tail) value

// int -> int
let rec countHits n =
    let dartList = throwDarts n |> List.map (fun x -> isHit x)
    listLength dartList
    helper dartList 0

// int -> int
let estimatePi n =
    let numHits = countHits n
    printfn "number of counted hits: %d" numHits
    let probability = (float numHits) / (float n)
    let result = (4.0 * probability)
    result

[<EntryPoint>]
let main argv = 
    printfn "MY PI Estimate is: %f" (estimatePi 1000)
    0 // return an integer exit code
