# Sky Colors
	1. Elwynn, normale Skybox Elwynn
	2. Duskwood-like, blau Duskwood
	3. rot/brauner Himmel, trüb, Westfall
	4. Sieht aus wie Elwynn
	5. afaik Deadwind Pass, blau/grauer Himmel
	6. afaik Blasted Lands, lila/blau
	7. Burning Stepps, schwarz/rot
	8. Sieht aus wie Elwynn, trübe Sicht
	9. Elwynn-like, bissl rötlich
	10. Klarer gräulicher Himmel, gute Sicht, <Screenshot>
	11. bissl trübe Sicht, aber sternenklarer himmel
	12. düster, größtenteils bewölkt
	13. trübe Sicht, bisschen bewölkt
	14. afaik Swamp of Sorrows
	15. blaue trübe Sicht, Duskwood-like
	16. Elwynn-like, bissl dunkler
	17. graue, trübe Sicht, Deadwindpass-like
	18. Duskwood-like
	19. total düster, rötlicher Himmel <Snapshot>
	20. nochma Burning-Stepps-like
	640: Weltuntergangs-Skybox^^
	704: Shadowmoon Valley, grüne Wolken mit Meteoren
	723: Bild
	240: GM island ?
	334: Bild
	187 - 190 = Emerald dream skyboxes
	
	edit: hier ein paar skyboxen von nen mm0wned typ
	380 - Saturated blue sky ,with green fog.
	383 - Saturated dark blue sky.
	385 - Purple sky.
	441 - Dull green sky.
	489 - Dark navy blue sky.
	491 - Purple sky, with yellow/white clouds.
	505 - Saturated yellowish/black/tan sky, with maroon overlay.
	506 - Blue/green sky, with purplish lighting.
	518 - Black to grey sky, with bright green fog/lighting.
	553 - Netherstorm.

## Skybox Real Position Formula

	private function RealPos(POS)
		return (32 * 533.3333) - POS
	end function

There are 64 tiles in each map, each tile is 533.3333 big, but you have to do 32 tiles because there is a +/-. Each ADT tile is 533.3333 big, and there can be 64x65 tiles in  each map. That means the world can be 4133.3333312 wide.