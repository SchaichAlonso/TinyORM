"use strict";(self.webpackChunktinyorm_github_io=self.webpackChunktinyorm_github_io||[]).push([[8],{3905:function(e,t,n){n.d(t,{Zo:function(){return p},kt:function(){return m}});var r=n(7294);function a(e,t,n){return t in e?Object.defineProperty(e,t,{value:n,enumerable:!0,configurable:!0,writable:!0}):e[t]=n,e}function i(e,t){var n=Object.keys(e);if(Object.getOwnPropertySymbols){var r=Object.getOwnPropertySymbols(e);t&&(r=r.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),n.push.apply(n,r)}return n}function o(e){for(var t=1;t<arguments.length;t++){var n=null!=arguments[t]?arguments[t]:{};t%2?i(Object(n),!0).forEach((function(t){a(e,t,n[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(n)):i(Object(n)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(n,t))}))}return e}function l(e,t){if(null==e)return{};var n,r,a=function(e,t){if(null==e)return{};var n,r,a={},i=Object.keys(e);for(r=0;r<i.length;r++)n=i[r],t.indexOf(n)>=0||(a[n]=e[n]);return a}(e,t);if(Object.getOwnPropertySymbols){var i=Object.getOwnPropertySymbols(e);for(r=0;r<i.length;r++)n=i[r],t.indexOf(n)>=0||Object.prototype.propertyIsEnumerable.call(e,n)&&(a[n]=e[n])}return a}var c=r.createContext({}),d=function(e){var t=r.useContext(c),n=t;return e&&(n="function"==typeof e?e(t):o(o({},t),e)),n},p=function(e){var t=d(e.components);return r.createElement(c.Provider,{value:t},e.children)},s={inlineCode:"code",wrapper:function(e){var t=e.children;return r.createElement(r.Fragment,{},t)}},u=r.forwardRef((function(e,t){var n=e.components,a=e.mdxType,i=e.originalType,c=e.parentName,p=l(e,["components","mdxType","originalType","parentName"]),u=d(n),m=a,f=u["".concat(c,".").concat(m)]||u[m]||s[m]||i;return n?r.createElement(f,o(o({ref:t},p),{},{components:n})):r.createElement(f,o({ref:t},p))}));function m(e,t){var n=arguments,a=t&&t.mdxType;if("string"==typeof e||a){var i=n.length,o=new Array(i);o[0]=u;var l={};for(var c in t)hasOwnProperty.call(t,c)&&(l[c]=t[c]);l.originalType=e,l.mdxType="string"==typeof e?e:a,o[1]=l;for(var d=2;d<i;d++)o[d]=n[d];return r.createElement.apply(null,o)}return r.createElement.apply(null,n)}u.displayName="MDXCreateElement"},5136:function(e,t,n){n.r(t),n.d(t,{assets:function(){return p},contentTitle:function(){return c},default:function(){return m},frontMatter:function(){return l},metadata:function(){return d},toc:function(){return s}});var r=n(7462),a=n(3366),i=(n(7294),n(3905)),o=["components"],l={sidebar_position:1,hide_table_of_contents:!0,description:"Library dependencies are MySQL Connector/C 8, range-v3 0.11.0, tabulate and the Qt framework version used during development was 5.15.2 and >= 6.2. The code was developed on MSVC 16.9-16.11.13, GCC 10.2-11.3, and Clang 11-14."},c="Dependencies",d={unversionedId:"dependencies",id:"dependencies",title:"Dependencies",description:"Library dependencies are MySQL Connector/C 8, range-v3 0.11.0, tabulate and the Qt framework version used during development was 5.15.2 and >= 6.2. The code was developed on MSVC 16.9-16.11.13, GCC 10.2-11.3, and Clang 11-14.",source:"@site/docs/dependencies.mdx",sourceDirName:".",slug:"/dependencies",permalink:"/dependencies",draft:!1,editUrl:"https://github.com/silverqx/TinyORM-github.io/edit/main/docs/dependencies.mdx",tags:[],version:"current",sidebarPosition:1,frontMatter:{sidebar_position:1,hide_table_of_contents:!0,description:"Library dependencies are MySQL Connector/C 8, range-v3 0.11.0, tabulate and the Qt framework version used during development was 5.15.2 and >= 6.2. The code was developed on MSVC 16.9-16.11.13, GCC 10.2-11.3, and Clang 11-14."},sidebar:"tinyormSidebar",previous:{title:"Prologue",permalink:"/"},next:{title:"Supported Compilers",permalink:"/supported-compilers"}},p={},s=[{value:"Install dependencies",id:"install-dependencies",level:3}],u={toc:s};function m(e){var t=e.components,n=(0,a.Z)(e,o);return(0,i.kt)("wrapper",(0,r.Z)({},u,n,{components:t,mdxType:"MDXLayout"}),(0,i.kt)("h1",{id:"dependencies"},"Dependencies"),(0,i.kt)("p",null,"The code was developed on MSVC 16.9-16.11.13, GCC 10.2-11.3, and Clang 11-14, so may be assumed it will work on future releases of these compilers. Minimum required ISO C++ standard is c++20.\nThe Qt framework version used during development was 5.15.2 and >= 6.2."),(0,i.kt)("ul",null,(0,i.kt)("li",{parentName:"ul"},"minimum ISO C++ standard is c++20"),(0,i.kt)("li",{parentName:"ul"},(0,i.kt)("a",{parentName:"li",href:"https://www.qt.io/download-qt-installer"},"Qt Framework 5.15.2")," - ",(0,i.kt)("a",{parentName:"li",href:"https://doc.qt.io/qt-5/qtcore-module.html"},(0,i.kt)("inlineCode",{parentName:"a"},"QtCore"))," and ",(0,i.kt)("a",{parentName:"li",href:"https://doc.qt.io/qt-5/qtsql-index.html"},(0,i.kt)("inlineCode",{parentName:"a"},"QtSql"))," modules"),(0,i.kt)("li",{parentName:"ul"},">","= ",(0,i.kt)("a",{parentName:"li",href:"https://www.qt.io/download-qt-installer"},"Qt Framework 6.2")," - ",(0,i.kt)("a",{parentName:"li",href:"https://doc.qt.io/qt-6/qtcore-module.html"},(0,i.kt)("inlineCode",{parentName:"a"},"QtCore"))," and ",(0,i.kt)("a",{parentName:"li",href:"https://doc.qt.io/qt-6/qtsql-index.html"},(0,i.kt)("inlineCode",{parentName:"a"},"QtSql"))," modules"),(0,i.kt)("li",{parentName:"ul"},(0,i.kt)("a",{parentName:"li",href:"https://github.com/ericniebler/range-v3"},"range-v3 0.11.0")),(0,i.kt)("li",{parentName:"ul"},(0,i.kt)("a",{parentName:"li",href:"https://github.com/p-ranav/tabulate"},"tabulate"))),(0,i.kt)("p",null,"Optional:"),(0,i.kt)("ul",null,(0,i.kt)("li",{parentName:"ul"},(0,i.kt)("a",{parentName:"li",href:"https://dev.mysql.com/downloads/c-api/"},"MySQL Connector/C 8")," - used only for the ",(0,i.kt)("a",{parentName:"li",href:"https://dev.mysql.com/doc/c-api/8.0/en/mysql-ping.html"},(0,i.kt)("inlineCode",{parentName:"a"},"mysql_ping()"))," function and provided by ",(0,i.kt)("a",{parentName:"li",href:"https://dev.mysql.com/downloads/mysql/"},"MySQL 8 Server"))),(0,i.kt)("h3",{id:"install-dependencies"},"Install dependencies"),(0,i.kt)("p",null,"On ",(0,i.kt)("inlineCode",{parentName:"p"},"Linux"),", you can install dependencies with the package manager."),(0,i.kt)("small",null,(0,i.kt)("pre",null,(0,i.kt)("code",{parentName:"pre",className:"language-text",metastring:"title='MySQL C library'",title:"'MySQL",C:!0,"library'":!0},"Arch   - pacman -S mariadb-libs\nGentoo - emerge dev-db/mysql (package.use: -server -perl)\nUbuntu - apt install libmysqlclient-dev\n")),(0,i.kt)("pre",null,(0,i.kt)("code",{parentName:"pre",className:"language-text",metastring:"title='range-v3 library (header only)'",title:"'range-v3",library:!0,"(header":!0,"only)'":!0},"Arch   - pacman -S range-v3\nGentoo - emerge dev-cpp/range-v3\nUbuntu - apt install librange-v3-dev\n")),(0,i.kt)("pre",null,(0,i.kt)("code",{parentName:"pre",className:"language-text",metastring:"title='ccache'",title:"'ccache'"},"Arch   - pacman -S ccache\nGentoo - emerge dev-util/ccache\nUbuntu - apt install ccache\n"))))}m.isMDXComponent=!0}}]);