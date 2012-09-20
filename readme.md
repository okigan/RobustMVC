#Intro
Well let's start off with a truism: software development is a complex endeavor.


There are a number of aspects to consider: overall architecture, physical layout, object lifetime
management, maintainability, performance, design (anti-)patterns, the list goes on. It is also hard to pick up a way to
approach these issues, as books and examples tend to focus on one of two of these aspects and take shortcuts in the
others.

In this article we will explorer a more complete example and provide a more robust framework that can be adapted to
meet further needs.

#Real world
Most discussions for creating and application start of with musing about Model-View-Controller [[MF]], but when push
comes to shove the implementation of a native application tends to start of from
MFC template that has a Document-View `architecture` or. Things tend to move downhill from there
and after a while busyness logic is intertwined with presentation and physical layout is intertwined with MFC, which in
essence is a big monolith with ever increasing build times and requiring more and more heroic effort to resolve issues
or introduce new functionality. Side note, a managed application templates are not much better, but check out my other article on managed application/WCF [[wcf technobabble]].

[MF]: http://martinfowler.com/eaaCatalog/modelViewController.html "MVC"
[wcf technobabble]: http://www.codeproject.com/Articles/107807/REST-Technobabble

#Robust Model-View-Controller
A few innovations were identified to improve upon the Model-View-Controller, but it seems much more important to improve
the way MVC is integrated with the platform used the build the application. More over that approach shall allow to reuse
the *actual* model and controller implementation across several platforms. Implementation for view(s) are generally more
dependent on the platform technology, but model/controller logic out of the way, it becomes much more straightforward to
re-implement the view (but see below how simplify that as well).

#The scope
So let's define the scope that would be covered in this example. Reminder we want to cover something reasonably complex
as to not skip aspects. Let's do something graphical, something using OpenGL, better yet lets use something a bit more
recent -- shaders. Usually applications are not build from scratch, so let's use some external libraries and see how
that fits in: boost [2] is a popular one, as well as glew [3] to help out with OpenGL. How about maintainability: let's
establish separate modules for corresponding functionality and allow to build unit tests for each.

Now let's pick the actual platform and actually define parts that be platform independent. Model and Controller shall be
platform independent. The rendering of views will be OpenGL and as OpenGL is cross platform, we'll keep the rendering
part cross platform as well.

Now the application itself -- the glue that bring everything together. We'll two of these, one will use MFC and the
other a standalone webserver. Why to choose MFC, because **you** most likely know (the few) good parts it has to offer
and the things that should be avoided. Why target a stand alone webserver, obviously because web connected frameworks
extremely useful. Also it is also diametrically opposite of a desktop MFC application, so that should show robustness of
the approach.

To top it off, documentation shall be generated automatically and (if we get time) source code analysis and code
coverage.

# Preview #
Sometimes it's better to start off with desert, in this case let's look at snapshots:

<table>
<tr>
<td>
<img src="https://raw.github.com/okigan/OpenGL/master/Documentation/images/application.png" width="350" height="350" />
</td>
<td>
<img src="https://raw.github.com/okigan/OpenGL/master/Documentation/images/browser.png" width="350" height="350" />
</td>
</tr>

<tr>
<td>
Application using OpenGL shaders
</td>
<td>
Webserver and Chrome browser
</td>
</tr>
</table>





# Intergration #
![](https://raw.github.com/okigan/OpenGL/master/Documentation/images/overview.png)
