This is where I will be validating my understanding in Ray tracing from the ground up, by walking through my thought process while building a CPU Ray tracer with C++.

<hr>

A Ray tracer needs rays. A Ray is just an infinite set of points that lie along a line. We can define a Ray as


> p = o + dt


where 
- p is any point on the line
- o is the origin of line (3D point)
- d is the direction of the line (3D vector)
- t is a scalar where -∞ <= t <= ∞

We need something for the rays to collide with. To start, let's add a Sphere. A Sphere can be defined as

> (x - a)<sup>2</sup> + (y - b)<sup>2</sup> + (z - c)<sup>2</sup> = r<sup>2</sup>

in 3D space. Thinking in terms of the 3D structures we're working with, it can be rewritten as 

> (p - c<sub>s</sub>)<sup>2</sup> = r<sup>2</sup>

So to compute the intersection of a Ray and a Sphere, we just need to find the points that satisfy both equations. I've written a mathematical derivation to explain this:

![Ray-Sphere intersection derived](/writeup/Ray-Sphere-intersection-derived.png)

If you don't feel like following the derivation, I basically substitute the Ray equation into the Sphere, and expand and rearrange it to form at<sup>2</sup> + bt + c = 0, which means we can apply the quadratic formula to it.

Using the discriminant b<sup>2</sup> + 4ac we can find how many solutions there are to this problem:

- If the discriminant is negative, there is no solution. 
  - This means the Ray misses the Sphere.
- If the discriminant is positive, there are two solutions.
  - This means the Ray intersects the Sphere in two places; we can take the smallest positive t to get the closest point to the Camera. 
- If the discriminant is zero, there is one solution.
  - This means the Ray is tangential to the Sphere, hitting it at a single point on the edge of the rendered image.

If you'd like a more visual explanation, here's a diagram depicting the geometry between a Sphere and an intersecting Ray (with slightly different variable names - sorry!):

![Ray-Sphere intersection](/writeup/Ray-Sphere-intersection-visualized.png)


In this case, we're using the circle equation instead of the Sphere equation. The circle is defined as the circular bisection of the Sphere that touches t1, t2, and the center of the circle. 

To be continued...