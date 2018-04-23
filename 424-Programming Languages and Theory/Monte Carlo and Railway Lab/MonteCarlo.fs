//
// type:Point - DUH!
type Point = { X: float; Y: float; }

//type HitResult - DUH!
type HitResult =
    | Hit
    | Miss of float

// type: float->float->Point (takes 2 floats, returns 1 Point)
let makePoint x y =
    {X = x; Y = y;}


let r = System.Random()

// type: unit -> Point (work around cuz of random seeding, have mercy!)
let randomPoint() =
    let x = r.NextDouble()
    let y = r.NextDouble()
    makePoint x y

//type: int -> Point list 
let throwDarts n =
    List.init n (fun index -> randomPoint())

// lil function i use in counthits to print out, as implied, length of list...
// 'a list -> unit (had to look this one up... didn't really know what a printfn would output... spare me!)
let listLength coll =
     coll |> List.length |> printfn "List of list is: %d"

//type: Point -> HitResult (i forget why i had to ensure point was Point i think it was to make the compiler happy or something dumb like that)
let isHit (point:Point) = 
    let circleBarrier = 
        (pown point.X 2) + (pown point.Y 2)
    //printfn "The value of this point is: %A" circleBarrier
    if circleBarrier <= 1.0 then
        HitResult.Hit
    else
        HitResult.Miss (circleBarrier - 1.0)

//type: HitResult list -> int -> int, a helper function I wrote to be used in CountHits... takes (1 HitResultList and 1 int and returns 1 int)
// makes use of some sexy tail end recursion (+1 extra credit maybe?)
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


// int -> int
//note the use of the above helper function which is of type HitResult list -> int -> int
let countHits n =
    let dartList = throwDarts n |> List.map (fun x -> isHit x)
    listLength dartList
    helper dartList 0

// int -> float
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
