### Introduction


The Native OSGi project is a collaborative effort to write, test, and implement the Java OSGi
specifications in C and C++ with a focus on interoperability between C, C++ and Java.

 - Doxygen documentation for a [C++ API draft](http://nativeosgi.org/documentation/cpp_api/html/index.html)

### Rationale for a native OSGi API

 - C and C++ as programming languages are here to stay (whether you like it or not).
 - Traditional application domains (for example in medical imaging, embedded devices, sensor networks, etc.)
   often use native languages and possibly rely on a large native (legacy) code-base.
 - Native developers designing scalable, modular platforms for a dynamically reconfigurable (embedded)
   environment need a light-weight yet powerful module system (just like Java developers).

### Benefits of a native OSGi framework

 - Builds upon a mature API design for a dynamic environment, supporting native developers in creating
   modular platforms.
 - Allows the creation of a hybrid Java and C/C++ architecture (via remote services) as an alternative to JNI.
 - Maximizes the performance per watt ratio for (embedded) devices.
 - Eases future software migrations of native components to a Java OSGi-based system.
