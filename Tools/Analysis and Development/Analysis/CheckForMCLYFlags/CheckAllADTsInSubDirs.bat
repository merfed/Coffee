@echo off

set checker=CheckForMCLYFlags.exe

cd ahnqiraj
echo checking ahnqiraj
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\ahnqiraj.res
cd ..
cd ahnqirajtemple
echo checking ahnqirajtemple
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\ahnqirajtemple.res
cd ..
cd azeroth
echo checking azeroth
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\azeroth.res
cd ..
cd Azjol_LowerCity
echo checking Azjol_LowerCity
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\Azjol_LowerCity.res
cd ..
cd Azjol_Uppercity
echo checking Azjol_Uppercity
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\Azjol_Uppercity.res
cd ..
cd blacktemple
echo checking blacktemple
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\blacktemple.res
cd ..
cd blackwinglair
echo checking blackwinglair
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\blackwinglair.res
cd ..
cd bladesedgearena
echo checking bladesedgearena
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\bladesedgearena.res
cd ..
cd cavernsoftime
echo checking cavernsoftime
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\cavernsoftime.res
cd ..
cd ChamberOfAspectsBlack
echo checking ChamberOfAspectsBlack
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\ChamberOfAspectsBlack.res
cd ..
cd dalaranarena
echo checking dalaranarena
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\dalaranarena.res
cd ..
cd DalaranPrison
echo checking DalaranPrison
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\DalaranPrison.res
cd ..
cd deadminesinstance
echo checking deadminesinstance
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\deadminesinstance.res
cd ..
cd DeathKnightStart
echo checking DeathKnightStart
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\DeathKnightStart.res
cd ..
cd development_nonweighted
echo checking development_nonweighted
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\development_nonweighted.res
cd ..
cd DrakTheronKeep
echo checking DrakTheronKeep
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\DrakTheronKeep.res
cd ..
cd emeralddream
echo checking emeralddream
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\emeralddream.res
cd ..
cd expansion01
echo checking expansion01
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\expansion01.res
cd ..
cd ExteriorTest
echo checking ExteriorTest
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\ExteriorTest.res
cd ..
cd GunDrak
echo checking GunDrak
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\GunDrak.res
cd ..
cd hellfirerampart
echo checking hellfirerampart
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\hellfirerampart.res
cd ..
cd hillsbradpast
echo checking hillsbradpast
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\hillsbradpast.res
cd ..
cd hyjalpast
echo checking hyjalpast
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\hyjalpast.res
cd ..
cd kalimdor
echo checking kalimdor
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\kalimdor.res
cd ..
cd karazahn
echo checking karazahn
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\karazahn.res
cd ..
cd monasteryinstances
echo checking monasteryinstances
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\monasteryinstances.res
cd ..
cd netherstormbg
echo checking netherstormbg
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\netherstormbg.res
cd ..
cd Nexus80
echo checking Nexus80
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\Nexus80.res
cd ..
cd NexusRaid
echo checking NexusRaid
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\NexusRaid.res
cd ..
cd northrend
echo checking northrend
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\northrend.res
cd ..
cd NorthrendBG
echo checking NorthrendBG
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\NorthrendBG.res
cd ..
cd orgrimmararena
echo checking orgrimmararena
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\orgrimmararena.res
cd ..
cd pvplordaeron
echo checking pvplordaeron
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\pvplordaeron.res
cd ..
cd pvpzone01
echo checking pvpzone01
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\pvpzone01.res
cd ..
cd pvpzone02
echo checking pvpzone02
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\pvpzone02.res
cd ..
cd pvpzone03
echo checking pvpzone03
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\pvpzone03.res
cd ..
cd pvpzone04
echo checking pvpzone04
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\pvpzone04.res
cd ..
cd pvpzone05
echo checking pvpzone05
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\pvpzone05.res
cd ..
cd QA_DVD
echo checking QA_DVD
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\QA_DVD.res
cd ..
cd razorfendowns
echo checking razorfendowns
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\razorfendowns.res
cd ..
cd razorfenkraulinstance
echo checking razorfenkraulinstance
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\razorfenkraulinstance.res
cd ..
cd schoolofnecromancy
echo checking schoolofnecromancy
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\schoolofnecromancy.res
cd ..
cd shadowfang
echo checking shadowfang
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\shadowfang.res
cd ..
cd stratholme
echo checking stratholme
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\stratholme.res
cd ..
cd Stratholme Raid
echo checking Stratholme Raid
for %%i in (*.adt) do ..\%checker% "%%i" >> "..\Stratholme Raid.res"
cd ..
cd StratholmeCOT
echo checking StratholmeCOT
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\StratholmeCOT.res
cd ..
cd sunwell5manfix
echo checking sunwell5manfix
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\sunwell5manfix.res
cd ..
cd sunwellplateau
echo checking sunwellplateau
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\sunwellplateau.res
cd ..
cd tanarisinstance
echo checking tanarisinstance
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\tanarisinstance.res
cd ..
cd Ulduar70
echo checking Ulduar70
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\Ulduar70.res
cd ..
cd Ulduar80
echo checking Ulduar80
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\Ulduar80.res
cd ..
cd UlduarRaid
echo checking UlduarRaid
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\UlduarRaid.res
cd ..
cd UtgardePinnacle
echo checking UtgardePinnacle
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\UtgardePinnacle.res
cd ..
cd Valgarde70
echo checking Valgarde70
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\Valgarde70.res
cd ..
cd zul'gurub
echo checking zul'gurub
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\zul'gurub.res
cd ..
cd zulaman
echo checking zulaman
for %%i in (*.adt) do ..\%checker% "%%i" >> ..\zulaman.res
cd ..