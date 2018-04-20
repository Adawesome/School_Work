// Learn more about F# at http://fsharp.org
// See the 'F# Tutorial' project for more help.

// Suppose I have a website where a user is attempting to register an account. I can 
// encapsulate such a request as a record:

type RegistrationAttempt = {username: string; email: string}

// As in:
let reg1 = {username = "neal"; email = "neal.terrell@csulb.edu"}

// Suppose I want to write a function to validate a registration attempt, checking against
// many rules for new accounts. In an OOP language, it might look something like this:

// I can then build a single monolithic "validate" function:
let validateRegistration reg =
    let {username = name; email = em} = reg
    if String.length name = 0 then
        "Missing a username"
    elif not (em.Contains("@")) then
        "Invalid email address"
    else
        "Valid"


(*
This solution has many problems:
1. The monolithic approach is difficult to maintain in general. As the number of validation 
    requirements grows, the if/elif chain grows to an unmaintainable size.
2. The return type (string) is unideal. We want to return a message if the validation fails,
    or some kind of True value if it succeeds. "Valid" as a string is an ugly hack, and the type
    system can't do us any favors here.
3. In a larger system, we might also want to do things like normalize the email address
    or otherwise transform the input when validation succeeds. The return type again hinders us.
A better design would incorporate a better return type that could discriminate between success
and failure, and would break the the validation requirements into individual functions.
Let's try it!
*)


// A better return type:
type ValidationResult =
    | Success of RegistrationAttempt
    | Failure of string

// A function can now return a ValidationResult, indicating success or failure. We can also
// start composing our validation functions.
let usernameExists reg =
    if reg.username.Length > 0 then
        Success reg
    else
        Failure "Username must not be blank"
// username: RegistrationAttempt -> ValidationResult

let emailHasAtSign reg = 
    if reg.email.Contains("@") then
        Success reg
    else
        Failure "Email address must contain a @"

let emailHasLocalPart reg =
    if reg.email.IndexOf("@") > 0 then
        Success reg
    else
        Failure "Email address does not have a local-part"

// Ideally, we would compose these functions such that failing the first validation
// means we don't attempt any later validations. Picture this like a railroad "switch":
// one track is the Success track, leading from one validation function to the next, as
// long as the validations are successful. The other track is the Failure track, which
// a validation function can send us to if the validation fails, and once we are on the 
// Failure track, we can never get back to the Success track.

// F# programmers call this pattern "Railway oriented programming": viewing the composition
// of functions as having more than one "track", with each function being responsible
// for moving to the correct track based on input.

// Picture time!

// Problem: we would love to compose our two validation functions, so one flows to the 
// next. But F# complains about the following line. Why?

// let validate2 = usernameExists >> emailHasAtSign

// usernameExists outputs a RegistrationResult, but emailHasAtSign expects a RegistrationAttempt,
// because emailHasAtSign is on the "Success" track and doesn't want to be bothered by
// failed attempts. In railway terminology, we call these "switches" -- they take a single
// track input and spit out a two-track output, which means we can't connect the output
// of one to the input of another... without some help.

let connectValidations reg =
    match usernameExists reg with
    | Success s -> emailHasAtSign s
    | Failure f -> Failure f

// Try it!
connectValidations {username = ""; email="a@b.com"} |> printfn "%A"
connectValidations {username = "asdf"; email="b.com"} |> printfn "%A"
connectValidations reg1 |> printfn "%A"

// But this is too specific; we want something more generic. In railway terms, we want
// a means of converting an existing switch function to a two-track input, which could then
// accept the two-track output of another function. We call this concept "bind": given a switch
// function and a two-track input, only call the switch function if the input is on the Success 
// track; otherwise bypass the switch function.

let bind switchFunction twoTrackInput =
    match twoTrackInput with
    | Success s -> switchFunction s
    | Failure f -> Failure f

// Now, we have a more generic way of connecting switch functions
let validate2 =
    usernameExists
    >> bind emailHasAtSign
    >> bind emailHasLocalPart

validate2 reg1 |> printfn "\n\nvalidate2 with bind:\n%A"


// Binding is so common that we can introduce an operator to make our life easier.
let (>=>) switch1 switch2 reg =
    bind switch2 (switch1 reg)

// And now we get
let validate3 =
    usernameExists
    >=> emailHasAtSign
    >=> emailHasLocalPart

let existingAccounts = ["hmdah@ultra.com"; "nealTerrell@csulb.com"; "letsaGO@mario.com"; "saitama@opm.com"; "tayay@power.com"]
let blacklistedDomains = ["mailinator.org"; "throwaway.com"]

//type RegistrationAttempt = {username: string; email: string}
let uniqueEmail attempt =
    let {username = name; email = em} = attempt
    match em with
    | List.head existingAccounts -> "hmdah"
    | _ -> "ouch"
[<EntryPoint>]
let main argv = 
    printfn "%A" argv
    0 // return an integer exit code
