# HL2_Combine_Kick_ThrowBack_Grenades
[ Techinical Differences Between The TrowBack and Kick]

THROWBACK

I wanted this to feel like a tactical, well-thought-out decision, so here’s what happens:
- The Soldier will look at the grenade 
- Crouch (Default animation)
- "Pick" the Grenade (Actually he's using the Gravity Gun Pull Code/Mechanic of the game)
- Reset the timer(Automatic)
- Look at the player
- Throws the grenade back to him
  
- The default animations were so slow that the grenade allways exploded on the soldier hand
- So I also used the Gravity Gun pull game mechanic that by default reset the enemy grenade timer when you pick one
- The lore reason I came up to justify this is because the Combine Soldiers have a software on their helmets that sends a signal that resets the grenade timer.

//////////////////////////////
//////////////////////////////

KICK 

I wanted this to look like a desperate act, so here’s what happens:
- The Soldier will look at the grenade 
- Will kick it away in the current direction his facing
- The Timer of the grenade will not be reseted

//////////////////////////////
//////////////////////////////

[ How To Find The Custom Logics]

Just hit Ctrl+F and search for the word MODIFICATION. That’ll take you straight to the custom parts of the code, so you can easily see what’s new versus what’s original Valve code. I also added comments to help explain the purpose of each line I created, making it easier for anyone to understand the goals behind the changes.

//////////////////////////////
//////////////////////////////
//////////////////////////////
//////////////////////////////


[ Brief Story]

Code that gives Half-Life 2 Combine Soldiers the ability to throw back or kick enemy grenades away
This is my first project in modding. I thought it would be cool for the Combine soldiers to have this ability, so I went ahead and tried to implement it.
At first, my goal was simply to enhance my own game experience. But then I realized it could be fun to share it with other people or creators who might like what they see.

[ Making Things Clear ]

I want to be upfront: I don’t actually know how to code in C++, especially not in SDK2013. I’m not a professional coder or even particularly smart, and I don’t want anyone to think that I am. I used GeminiPro to help me write the code.

But it’s important to say that the code only reached its current state because these were the ideas, logic, and approaches that I thought were best. I guided the AI to write it correctly, using only code and logic that SDK2013 recognizes.

Really, try it yourself. Ask it to write some custom code without any guidance, and you’ll see the mess it comes up with. That’s why I believe this project still reflects my own thinking and problem-solving. Even though I leaned on AI to help write the code, the ideas and logic were mine—I just didn’t know how to translate them into code on my own

By saying all this, I just want to make it clear: I don’t claim the code as mine. I don’t think of it that way. I simply wanted to create a feature and share it, in case others liked it and wanted to use it too.

So feel free to use it exactly as it is, or treat it as a blueprint to build your own version. Either way, I hope it helps!

[ Functionality ]

The code is working perfectly—it doesn’t cause crashes or have any noticeable bugs. I did my best to clean those up.

This behavior only triggers if the grenade is really close to the soldier, almost touching his foot. If the grenade lands even just 1 unit farther away, the soldier will stick to his default behavior and run from it.

Of course, you can tweak these values if you want. 

