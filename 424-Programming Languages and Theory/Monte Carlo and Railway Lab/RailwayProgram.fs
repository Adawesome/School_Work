//deliverables does not say we need to say each type so...
type RegistrationAttempt = {username: string; email: string}

let reg1 = {username = "neal"; email = "neal.terrell@csulb.edu"}

let validateRegistration reg =
    let {username = name; email = em} = reg
    if String.length name = 0 then
        "Missing a username"
    elif not (em.Contains("@")) then
        "Invalid email address"
    else
        "Valid"

type ValidationResult =
    | Success of RegistrationAttempt
    | Failure of string
 

let usernameExists reg =
    if reg.username.Length > 0 then
        Success reg
    else
        Failure "Username must not be blank"

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
let connectValidations reg =
    match usernameExists reg with
    | Success s -> emailHasAtSign s
    | Failure f -> Failure f

let bind switchFunction twoTrackInput =
    match twoTrackInput with
    | Success s -> switchFunction s
    | Failure f -> Failure f

let validate2 =
    usernameExists
    >> bind emailHasAtSign
    >> bind emailHasLocalPart

let (>=>) switch1 switch2 reg =
    bind switch2 (switch1 reg)
 
let validate3 =
    usernameExists
    >=> emailHasAtSign
    >=> emailHasLocalPart
 //___________________________________________________________________________
 // CODE BELOW IS PROGRAM 2::CODE ABOVE IS GIVEN TO US
 //___________________________________________________________________________
let existingAccounts = ["adawesome@hmdah.com"; "Hmdah@ultra.com"; "NealTerrell@school.com"; "youpipeditnice@forward.com"; "IceClimbers@com.com"]
let blacklistedDomains = ["mailinator.org"; "throwawaymail.com"]
let uniqueEmail (reg:RegistrationAttempt) =
    if (List.contains(reg.email) existingAccounts) then
        Failure "Email is not unique..."
    else
        Success reg

let emailNotBlacklisted reg =
    let domainStart = reg.email.IndexOf('@')
    let domainString = reg.email.Substring(domainStart+1)
    if (List.contains(domainString) blacklistedDomains) then
        Failure "The email domain is blacklisted..."
    else
        Success reg

let switch f reg =
    f reg |> Success

let lowercaseEmail reg =
    let lowered = reg.email.ToLower()
    let newReg = {username = reg.username; email = lowered}
    newReg

let canonicalizeEmail reg =
    let domainStart = reg.email.IndexOf('@')
    let domain = reg.email.Substring(domainStart+1)
    let shrinkReg = reg.email.[0..domainStart]
    let replacer = shrinkReg.Replace(".", "")
    let replacer2 = replacer.Replace("-", "")
    {username = reg.username; email = replacer2+domain}
 
//surprisingly enough (if you recall this was called "trainWreck") it worked first try~~
let finalValidateRegistration =
    validate3
    >=> switch canonicalizeEmail
    >=> switch lowercaseEmail
    >=> emailNotBlacklisted
    >=> uniqueEmail


[<EntryPoint>]
let main argv =
    printfn "Testing my code now...\n"
    let notUnique = {username = "swag"; email = "adawesome@hmdah.com"}
    let blackAccount = {username = "adam"; email = "ScottRoberts@mailinator.org"}
    let noLocal = {username = "kneel"; email = "@school.com"}
    let noAt = {username = "wow"; email = "https://gyazo.com/c1d72c1a5d6941ecf38b0683423ea93a"}
    let noUser = {username = ""; email = "too.cool@iceage.com"}
    let curious = {username = "adamhmdah"; email = ""}
    let winner = {username = "Teenager"; email = "xX420S14yerxX@myspace.com"}
    printfn "Test for not unique"
    finalValidateRegistration notUnique |> printfn "Result of test: %A\n"
    printfn "Test for blacklisted"
    finalValidateRegistration blackAccount |> printfn "Result of test: %A\n"
    printfn "Test for no local part"
    finalValidateRegistration noLocal |> printfn "Result of test: %A\n"
    printfn "Test for no @"
    finalValidateRegistration noAt |> printfn "Result of test: %A\n"
    printfn "Test for empty username"
    finalValidateRegistration noUser |> printfn "Result of test: %A\n"
    printfn "Test for empty email"
    finalValidateRegistration curious |> printfn "Result of test: %A\n"
    printfn "Working test"
    finalValidateRegistration winner |> printfn "Result of test: %A\n"
    0 // return an integer exit code
