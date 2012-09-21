# Introduction
In development of a sizable software product there are a number of
aspects to consider: overall architecture, physical layout, object
lifetime management, maintainability, performance, design
(anti-)patterns, the list goes on. It is also hard to pick up an
template that addresses these issues, as books and examples tend to
focus on one of two of these aspects and take shortcuts in the others.

In this article we will explorer a more complete example and provide a
more robust framework that can be adapted to meet further needs.

 
# Real world
Most discussions for creating and application start of with musing about
Model-View-Controller [[MF]], but most native application projects still
start of from MFC template that has a Document-View "architecture".
Things tend to move downhill from there and after a while business logic
is intertwined with presentation and physical layout is intertwined with
MFC. Sprinkle couple more external libraries and you've got a big
monolithic code base with ever increasing build times and requiring more
and more heroic effort to resolve issues or introduce new functionality.
Side note, a managed application templates are not much better, but
check out my other article on managed application/WCF [[wcf
technobabble]]. 

[MF]: http://martinfowler.com/eaaCatalog/modelViewController.html "MVC"
[wcf technobabble]: http://www.codeproject.com/Articles/107807/REST-Technobabble

# Robust Model-View-Controller
A few innovations were identified to improve upon the
Model-View-Controller [[MVVM]], but it seems much more important to
improve the way MVC is integrated with the platform/library used the
build the application. More over that approach shall allow to reuse the
*actual* model and controller implementation across several platforms.
Implementation for view(s) are generally more dependent on the 
presentation/visualization technology, but with model/controller logic 
out of the way, it becomes much more straightforward to re-implement the 
view (but see below how simplify that task by re-using visualization 
building blocks).

[mvvm]: http://en.wikipedia.org/wiki/Model_View_ViewModel
 
# The scope
So let's define the scope that would be covered in this example.
Reminder we want to cover something reasonably complex as to not skip
important aspects. Let's do something graphical, something using OpenGL, 
better yet lets use something a bit more recent -- shaders. Usually
applications are not build from scratch, so let's use some external
libraries and see how that fits in: boost [2] is a popular one, as well
as glew [3] to help out with OpenGL. How about maintainability and 
physical layout: establish separate modules for corresponding 
functionality and allow to build unit tests as well allow reuse in other 
projects.

Now let's pick the actual platform and actually define parts that will be
platform independent. Model and Controller shall be platform independent. 
The rendering of views will be OpenGL and as it is cross platform, 
we'll keep the rendering part cross platform as well.

Now the application itself -- the glue that bring everything together.
We'll employ two of these: one will use MFC and the other a standalone
webserver. Why to choose MFC, because **you** most likely already know 
the good and pain points about it. Why target a stand alone webserver, 
obviously because web connected frameworks extremely useful. But also 
because it is diametrically opposite of a desktop MFC application, so that 
should show robustness of the approach.

To top it off, documentation shall be generated automatically and (if we
get time) source code analysis and code coverage.

# Preview #
Sometimes it's better to start off with desert, in this case let's look at snapshots:

<table>
<tr>
<td>
<img src="https://raw.github.com/okigan/RobustMVC/master/Documentation/images/application.png" width="350" height="350" />
</td>
<td>
<img src="https://raw.github.com/okigan/RobustMVC/master/Documentation/images/browser.png" width="350" height="350" />
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





# Integration 
![](https://raw.github.com/okigan/RobustMVC/master/Documentation/images/overview.png)
![](https://raw.github.com/okigan/RobustMVC/master/Documentation/images/dep_doc.png)
![](https://raw.github.com/okigan/RobustMVC/master/Documentation/images/dep_view.png)
