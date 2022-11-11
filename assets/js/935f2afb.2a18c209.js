"use strict";(self.webpackChunktinyorm_org=self.webpackChunktinyorm_org||[]).push([[53],{1109:e=>{e.exports=JSON.parse('{"pluginId":"default","version":"current","label":"Next","banner":null,"badge":false,"noIndex":false,"className":"docs-version-current","isLast":true,"docsSidebars":{"tinyormSidebar":[{"type":"link","label":"\ud83d\udd25 Prologue","href":"/","docId":"README"},{"type":"link","label":"\ud83d\udd27 Dependencies","href":"/dependencies","docId":"dependencies"},{"type":"link","label":"\ud83d\ude80 Supported Compilers","href":"/supported-compilers","docId":"supported-compilers"},{"type":"category","label":"\u2728 Database","collapsible":true,"collapsed":true,"items":[{"type":"link","label":"Getting Started","href":"/database/getting-started","docId":"database/getting-started"},{"type":"link","label":"Query Builder","href":"/database/query-builder","docId":"database/query-builder"},{"type":"link","label":"Migrations","href":"/database/migrations","docId":"database/migrations"},{"type":"link","label":"Seeding","href":"/database/seeding","docId":"database/seeding"}]},{"type":"category","label":"\ud83c\udf89 TinyORM","collapsible":true,"collapsed":true,"items":[{"type":"link","label":"Getting Started","href":"/tinyorm/getting-started","docId":"tinyorm/getting-started"},{"type":"link","label":"Relationships","href":"/tinyorm/relationships","docId":"tinyorm/relationships"},{"type":"link","label":"Casts","href":"/tinyorm/casts","docId":"tinyorm/casts"}]},{"type":"category","label":"\ud83d\udea7 Building","collapsible":true,"collapsed":true,"items":[{"type":"link","label":"TinyORM","href":"/building/tinyorm","docId":"building/tinyorm"},{"type":"link","label":"Hello world","href":"/building/hello-world","docId":"building/hello-world"},{"type":"link","label":"Migrations","href":"/building/migrations","docId":"building/migrations"}]},{"type":"link","label":"\ud83d\udcc4 Features Summary","href":"/features-summary","docId":"features-summary"}]},"docs":{"building/hello-world":{"id":"building/hello-world","title":"Building: Hello world","description":"Hello world example created in the terminal and QtCreator IDE.","sidebar":"tinyormSidebar"},"building/migrations":{"id":"building/migrations","title":"Building: Migrations","description":"How to compile the TinyORM migrations (tom) c++ console application on Windows and Linux.","sidebar":"tinyormSidebar"},"building/tinyorm":{"id":"building/tinyorm","title":"Building: TinyORM","description":"How to compile the TinyORM c++ library on Windows and Linux.","sidebar":"tinyormSidebar"},"database/getting-started":{"id":"database/getting-started","title":"Database: Getting Started","description":"TinyORM makes interacting with a database extremely simple using raw SQL, a fluent query builder, and the TinyORM. It provides first-party support for three databases MySQL/MariaDB, PostgreSQL, and SQLite.","sidebar":"tinyormSidebar"},"database/migrations":{"id":"database/migrations","title":"Database: Migrations","description":"Migrations are like version control for your database, allowing your team to define and share the application\'s database schema definition. Migrations use the Schema facade that provides database agnostic support for creating and manipulating tables across all of TinyORM\'s supported database systems.","sidebar":"tinyormSidebar"},"database/query-builder":{"id":"database/query-builder","title":"Database: Query Builder","description":"TinyORM\'s database query builder provides a convenient, fluent interface to creating and running database queries. It can be used to perform most database operations in your application. The query builder uses QSqlQuery parameter binding to protect your application against SQL injection attacks. There is no need to clean or sanitize strings passed to the query builder as query bindings.","sidebar":"tinyormSidebar"},"database/seeding":{"id":"database/seeding","title":"Database: Seeding","description":"TinyORM includes the ability to seed your database with data using seed classes. The DatabaseSeeder class is considered as the root seeder. From this class, you may use the `call` method to run other seed classes, allowing you to control the seeding order.","sidebar":"tinyormSidebar"},"dependencies":{"id":"dependencies","title":"Dependencies","description":"Library dependencies are MySQL Connector/C 8, range-v3 >=0.11.0, tabulate and the Qt framework version used during development was 5.15.2 and >=6.2. The code was developed on MSVC 16.9-16.11, MSVC 17.2-17.3, GCC 10.2-12.2, and Clang 11-15.","sidebar":"tinyormSidebar"},"features-summary":{"id":"features-summary","title":"Features Summary","description":"List that fastly summarizes all TinyORM features.","sidebar":"tinyormSidebar"},"README":{"id":"README","title":"Prologue","description":"TinyORM is a modern c++ ORM library that makes interacting with a database extremely simple. It depends on the QtCore and QtSql libraries. The code is written in the modern c++20 way and is well tested with the unit and functional tests.","sidebar":"tinyormSidebar"},"supported-compilers":{"id":"supported-compilers","title":"Supported Compilers","description":"Platform requirements, supported compilers and build systems for TinyORM c++ library.","sidebar":"tinyormSidebar"},"tinyorm/casts":{"id":"tinyorm/casts","title":"TinyORM: Casting","description":"Attribute casting allow you to transform TinyORM attribute values when you retrieve them on model instances. For example, you may want to convert a `datetime` string that is stored in your database to the `QDateTime` instance when it is accessed via your TinyORM model.","sidebar":"tinyormSidebar"},"tinyorm/getting-started":{"id":"tinyorm/getting-started","title":"TinyORM: Getting Started","description":"TinyORM is an object-relational mapper (ORM) that makes it enjoyable to interact with your database. When using TinyORM, each database table has a corresponding \\"Model\\" that is used to interact with that table. In addition to retrieving records from the database table, TinyORM models allow you to insert, update, and delete records from the table as well.","sidebar":"tinyormSidebar"},"tinyorm/relationships":{"id":"tinyorm/relationships","title":"TinyORM: Relationships","description":"TinyORM relationships are defined as methods on your TinyORM model classes. Since relationships also serve as powerful query builders, defining relationships as methods provides powerful method chaining and querying capabilities.","sidebar":"tinyormSidebar"}}}')}}]);