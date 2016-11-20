# MaD's Piercing the Mists of Game Mechanics

## Hidden Spells & Units
> MoD Said:
> 
> While browsing the forums, I noticed a recent re-interest in finding & casting specific internal spells.

> Eventhought the easy way (using WoW API) of acquiring those spells in your hotkeys is gone thanks to Raov publicly exposing it,

> I wanted to let you know it's still very possible to achieve.

> As for getting hidden spells to show up, there is a major hint left in one of the screenshots (which I pick & edit for that matter) it's even written in plain letters,
just think for a second about how the game shows spells, display data has to be stored along with other informations somewhere in the game client...

> You can 'unlock' your client's interface, in order to show & sometimes interact with all internal hidden spells/auras affecting your character & other units such a players or NPCs in the game world, exposing many scripting features:

> Getting your game to show all spells/units require nothing but a clientside DBC patch.


![1](/images/1.jpg)

You can also tweak a few spell effects, like allowing casts while sitting, or, among others, give a few spells true unlimited range:

![2](/images/2.jpg)

This is not limited to spells, same can be done with all units, even ones that are not usually visible/targetable/interactible with:

![3](/images/3.jpg)

The point of the above screenshot is to show Darkmoon's unit's health (84 in this case when most hidden/uninteractible units have 1 health) and the absence of icon as it's not supposed to be targeted at all. You can also see I have usually unavailable spells in my hotkeys, such as Unstuck.

When you move around the game's world / cast spells / ride a mount or a vehicle / fight other units / etc, you character is affected by various auras, procs & other spell effects, which you can then cancel in some cases, and leads to interesting results: You can make surrounding summoned units like portals disappear, cancel the speed buff given my mounts and fly at walking speed, etc, list of possible is rather huge & would require extended testing.

Even though server side checked in most cases, playing around with SpellAttribs can lead to the most unexpected behaviors, from no being to cast at all to some really weird results in spell casts.

## Pet Battles
When it was first introduced, the Pet Battle system was all but flawless,
in fact totally insecure (Even though they were not documented yet, ALL of the C_PetBattles. API functions were unprotected by then) and filled with cheesy events.
It was a huge mess, throwing cages at full life opponent's pets (server side, opponent could see it), giving any Pet Battle Abilities to your current pet admitting you knew it's UUID was possible just by using those API functions. Things like capturing the same pet over and over (retaining breed & quality) was as easy as casting Mind Control or any other spell making you loose control on your character while successfully capturing one.

Summoning non-learned pets was sadly unavailable, even after modding the game client to do so.

By next patch, Blizzard had secured the most critical functions & fixed most bugs, which means times to force things to happen.

A few screenshots of various funny multi-interactions tests:

![4](/images/4.jpg)

Without any or very few modifications, I still found a few funny uses:

Move around & receive item while in Pet Battle:

![5](/images/5.jpg)

Evade Pet Battle, letting you move around freely with wallclimbing abilities until the Pet Battle ends, which can be infinite against wild pets:

![6](/images/6.jpg)

Another evade without health penalty to your pets for breaking battle, but can't Pet Battle until relog:

![7](/images/7.jpg)

![8](/images/8.jpg)

Nothing much you might say, but it was overall fun to try things out, and it leads to an interesting conclusion.

Pet Battles are mostly client side (anything but damage calculation & pet abilities) which means that with heavy modifications, they can serve a new purpose:

It can basically display whatever you want, and could be used as a theatre / machinima tool within WoW to play custom scenes!

## Mounts
> MoD Said:

> Crafting & using the Sky Claw & Pierre on live servers in 5.2/5.3 before they were officially implemented in 5.4 was possible using a private 'quest recovery' method, different from the old quest sharing exploit that was made public. (you have to know one thing, which I will develop later in the chapter on quests: Unless specificaly disabled/deleted/overwritten, ALL quests -including developement / test / available on alpha/beta PTRs but not hitting live- ARE available on live servers, meaning the quests ARE in the database, ready to be completed if you somehow find a way to acquire them and if you meet all the requirements)

> The plans of the new engineering items including the Sky Claw were obtained the way Blizzard had initially planned it for 5.2 engineers (quest required to kill Itoka, then interact with tomes inside the forge to get your plans, see screenshot)

> A total of 3 mounts were crafted, one for myself, one for Shamatix (toy-around partner), and another which was given the first day of 5.4, upon turning into Sky Golem, to someone I won't name, evenhought he is a user of that forum, since he bragged with it around Timeless Isle during patch day, and could have gotten us banned.

> If you still don't believe that's what happened and I'm faking things, that's your choice and I actually don't care much.

Mounts are a key element of the game, they are present since launch and basically every player owns and likes at least one. Most of them are common, some are rare & the center of all attentions, others simply never make it into the player's hands. There are several mounts that were planned or being worked on at some point, but finally were never made available to players.

When Blizzard released Patch 5.2, they pulled off the new engineering mount -the Sky Claw- at the last minute, pretending it was unusable due to bugs...

This is actually a lie. The true reason it was canceled is that even thought as extremely fun as it looked, the actual model & textures were totally outdated,
no where near the standarts Blizzard (or rather, their texture artists) set since Cataclysm, and definitely not looking like a 2013 mount. They simply couldn't release a "new" mount using an old WOTLK model.

The developement of the mount itself seems full of pain, it's a modified sandstone drake which is itself another modified 2-seater mount, no wonder why it had a few bugs when Blizzard in all it's lazyness feels like copying spells rather then making new ones. But appart from the usual 'loosing passager' upon zoning/phasing, and parachute upon dismount opening very randomly, it was fully working & fun to fly with (contrary to sky Golem!)

But how do I know you may ask ?

Back in April when 5.2 hit, I was able to retrieve the pulled off engineering quest using vOOdOO:

![9](/images/9.jpg)

From there, I was able to craft the 30 Stabilized Lightning Source using patience (never rolled the cooldown back!):

![10](/images/10.jpg)

And after 28 days finally craft the mount on live servers:

![11](/images/11.jpg)

(For those wondering why i'm at entrance, back in 5.2 The Thunder Forges were only partially implemented, the book game object to complete the quest was active while the actual anvils were not, and you actually had to stand in the entrance to be in the correct zone & use portable anvil, which was fixed in 5.3)

Here are a few screenshots of the mount itself in various settings:

![12](/images/12.jpg)

![13](/images/13.jpg)

![14](/images/14.jpg)

![15](/images/15.jpg)

Holding TOM_RUS, one of the first passagers ever:

![16](/images/16.jpg)

Since you don't like my screenshots, I spent some time making a small video using the very few clips & screens I had. I tried to make the whole thing funny to watch & follow the music, but since I suck at editing clips, don't expect wonders.
Hopefully you will still appreciate this brief view of this unique cancelled mount: [video](https://www.youtube.com/watch?v=4lPAYlE1O4Q).

This short video also shows Pierre crafted in 5.3 (in case you're missing the obvious, before it was officially released) and not displaying in Pet Journal, but could be still summoned once learned using a simple maccro.

Sadly, the mount's related item/spell/mount/recipe/reagants IDs got reused for the new engineering hideous mount everyone knows, the Sky Golem, featuring painfull animations & choppy flights...

RIP Sky Claw, one of the funniest mounts Blizzard ever made! 

More on other uncommon mounts will follow later.