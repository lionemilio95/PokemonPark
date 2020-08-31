# PokemonPark
This is a course project in UCSD using OpenGL to render a Pokemon world and Pokemons. My group member and I used several techniques including toon shading, physical collision detection using bounding spheres, procedurally generated terrain and plants and so on. 

We implement Diamond Algorithm to produce a terrain in the world and give it different color according to the depth of the point: points with high z value would be white, medium z value would be green, and low z value would be rock color. 
![Image of Terrain](https://github.com/lionemilio95/jiayue/blob/master/projectimg/finalpj.png)

There is a pool in the middle of the terrain, we used Bezier curve to realize the wave effect and put several pokemon inside of the pool. 
![Image of Bezier Curve](https://github.com/lionemilio95/jiayue/blob/master/projectimg/finalpjbc.png)


To prevent on pokemon cross over other pokemons, we add a bounding sphere to each pokemon to detect collision.  If collision happens, two pokemons would turn around and walk along the opposite direction. 
![Image of Bouding Sphere](https://github.com/lionemilio95/jiayue/blob/master/projectimg/finalpjbs.png)

Here is the Youtube link of our project demo video.
https://www.youtube.com/watch?v=23DuHyZ1Kdk&list=PLgrNWQ9zqY8Zn9lsHzvQWikCJrX5VUoYg&index=15
