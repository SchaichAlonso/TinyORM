"use strict";(self.webpackChunktinyorm_org=self.webpackChunktinyorm_org||[]).push([[8],{3905:(e,t,n)=>{n.d(t,{Zo:()=>p,kt:()=>u});var r=n(7294);function a(e,t,n){return t in e?Object.defineProperty(e,t,{value:n,enumerable:!0,configurable:!0,writable:!0}):e[t]=n,e}function i(e,t){var n=Object.keys(e);if(Object.getOwnPropertySymbols){var r=Object.getOwnPropertySymbols(e);t&&(r=r.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),n.push.apply(n,r)}return n}function l(e){for(var t=1;t<arguments.length;t++){var n=null!=arguments[t]?arguments[t]:{};t%2?i(Object(n),!0).forEach((function(t){a(e,t,n[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(n)):i(Object(n)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(n,t))}))}return e}function o(e,t){if(null==e)return{};var n,r,a=function(e,t){if(null==e)return{};var n,r,a={},i=Object.keys(e);for(r=0;r<i.length;r++)n=i[r],t.indexOf(n)>=0||(a[n]=e[n]);return a}(e,t);if(Object.getOwnPropertySymbols){var i=Object.getOwnPropertySymbols(e);for(r=0;r<i.length;r++)n=i[r],t.indexOf(n)>=0||Object.prototype.propertyIsEnumerable.call(e,n)&&(a[n]=e[n])}return a}var d=r.createContext({}),s=function(e){var t=r.useContext(d),n=t;return e&&(n="function"==typeof e?e(t):l(l({},t),e)),n},p=function(e){var t=s(e.components);return r.createElement(d.Provider,{value:t},e.children)},c={inlineCode:"code",wrapper:function(e){var t=e.children;return r.createElement(r.Fragment,{},t)}},m=r.forwardRef((function(e,t){var n=e.components,a=e.mdxType,i=e.originalType,d=e.parentName,p=o(e,["components","mdxType","originalType","parentName"]),m=s(n),u=a,h=m["".concat(d,".").concat(u)]||m[u]||c[u]||i;return n?r.createElement(h,l(l({ref:t},p),{},{components:n})):r.createElement(h,l({ref:t},p))}));function u(e,t){var n=arguments,a=t&&t.mdxType;if("string"==typeof e||a){var i=n.length,l=new Array(i);l[0]=m;var o={};for(var d in t)hasOwnProperty.call(t,d)&&(o[d]=t[d]);o.originalType=e,o.mdxType="string"==typeof e?e:a,l[1]=o;for(var s=2;s<i;s++)l[s]=n[s];return r.createElement.apply(null,l)}return r.createElement.apply(null,n)}m.displayName="MDXCreateElement"},5136:(e,t,n)=>{n.r(t),n.d(t,{assets:()=>d,contentTitle:()=>l,default:()=>c,frontMatter:()=>i,metadata:()=>o,toc:()=>s});var r=n(7462),a=(n(7294),n(3905));const i={sidebar_position:1,sidebar_label:"\ud83d\udd27 Dependencies",hide_table_of_contents:!0,description:"Library dependencies are MySQL Connector/C 8, range-v3 >=0.11.0, tabulate and the Qt framework version used during development was 5.15.2 and >=6.2. The code was developed on MSVC 16.9-16.11, MSVC 17.2-17.3, GCC 10.2-12.2, and Clang 11-15.",keywords:["c++ orm","dependencies","tinyorm"]},l="Dependencies",o={unversionedId:"dependencies",id:"dependencies",title:"Dependencies",description:"Library dependencies are MySQL Connector/C 8, range-v3 >=0.11.0, tabulate and the Qt framework version used during development was 5.15.2 and >=6.2. The code was developed on MSVC 16.9-16.11, MSVC 17.2-17.3, GCC 10.2-12.2, and Clang 11-15.",source:"@site/docs/dependencies.mdx",sourceDirName:".",slug:"/dependencies",permalink:"/dependencies",draft:!1,editUrl:"https://github.com/silverqx/TinyORM-github.io/edit/main/docs/dependencies.mdx",tags:[],version:"current",sidebarPosition:1,frontMatter:{sidebar_position:1,sidebar_label:"\ud83d\udd27 Dependencies",hide_table_of_contents:!0,description:"Library dependencies are MySQL Connector/C 8, range-v3 >=0.11.0, tabulate and the Qt framework version used during development was 5.15.2 and >=6.2. The code was developed on MSVC 16.9-16.11, MSVC 17.2-17.3, GCC 10.2-12.2, and Clang 11-15.",keywords:["c++ orm","dependencies","tinyorm"]},sidebar:"tinyormSidebar",previous:{title:"\ud83d\udd25 Prologue",permalink:"/"},next:{title:"\ud83d\ude80 Supported Compilers",permalink:"/supported-compilers"}},d={},s=[{value:"Required",id:"required",level:5},{value:"Optional",id:"optional",level:5},{value:"Install dependencies",id:"install-dependencies",level:3}],p={toc:s};function c(e){let{components:t,...n}=e;return(0,a.kt)("wrapper",(0,r.Z)({},p,n,{components:t,mdxType:"MDXLayout"}),(0,a.kt)("h1",{id:"dependencies"},"Dependencies"),(0,a.kt)("p",null,"The code was developed on MSVC 16.9-16.11, MSVC 17.2-17.3, GCC 10.2-12.2, and Clang 11-15, so may be assumed it will work on future releases of these compilers. Minimum required ISO C++ standard is c++20.\nThe Qt framework version used during development was 5.15.2 and >=6.2."),(0,a.kt)("h5",{id:"required"},"Required"),(0,a.kt)("ul",null,(0,a.kt)("li",{parentName:"ul"},"minimum ISO C++ standard is c++20"),(0,a.kt)("li",{parentName:"ul"},">","= ",(0,a.kt)("a",{parentName:"li",href:"https://www.qt.io/download-qt-installer"},"Qt Framework 5.15.2")," - ",(0,a.kt)("a",{parentName:"li",href:"https://doc.qt.io/qt-5/qtcore-module.html"},(0,a.kt)("inlineCode",{parentName:"a"},"QtCore"))," and ",(0,a.kt)("a",{parentName:"li",href:"https://doc.qt.io/qt-5/qtsql-index.html"},(0,a.kt)("inlineCode",{parentName:"a"},"QtSql"))," modules ",(0,a.kt)("small",{className:"darker"},"(MSVC 2019 binaries)")),(0,a.kt)("li",{parentName:"ul"},">","= ",(0,a.kt)("a",{parentName:"li",href:"https://www.qt.io/download-qt-installer"},"Qt Framework 6.2")," - ",(0,a.kt)("a",{parentName:"li",href:"https://doc.qt.io/qt-6/qtcore-module.html"},(0,a.kt)("inlineCode",{parentName:"a"},"QtCore"))," and ",(0,a.kt)("a",{parentName:"li",href:"https://doc.qt.io/qt-6/qtsql-index.html"},(0,a.kt)("inlineCode",{parentName:"a"},"QtSql"))," modules ",(0,a.kt)("small",{className:"darker"},"(MSVC 2019 binaries)")),(0,a.kt)("li",{parentName:"ul"},">","= ",(0,a.kt)("a",{parentName:"li",href:"https://github.com/ericniebler/range-v3"},"range-v3 0.11.0")),(0,a.kt)("li",{parentName:"ul"},(0,a.kt)("a",{parentName:"li",href:"https://github.com/p-ranav/tabulate"},"tabulate"))),(0,a.kt)("h5",{id:"optional"},"Optional"),(0,a.kt)("ul",null,(0,a.kt)("li",{parentName:"ul"},(0,a.kt)("a",{parentName:"li",href:"https://dev.mysql.com/downloads/c-api/"},"MySQL Connector/C 8")," - used only for the ",(0,a.kt)("a",{parentName:"li",href:"https://dev.mysql.com/doc/c-api/8.0/en/mysql-ping.html"},(0,a.kt)("inlineCode",{parentName:"a"},"mysql_ping"))," function and provided by ",(0,a.kt)("a",{parentName:"li",href:"https://dev.mysql.com/downloads/mysql/"},"MySQL 8 Server"))),(0,a.kt)("h3",{id:"install-dependencies"},"Install dependencies"),(0,a.kt)("p",null,"On ",(0,a.kt)("inlineCode",{parentName:"p"},"Linux"),", you can install dependencies with the package manager."),(0,a.kt)("small",null,(0,a.kt)("pre",null,(0,a.kt)("code",{parentName:"pre",className:"language-text",metastring:"title='MySQL C library'",title:"'MySQL",C:!0,"library'":!0},"Arch   - pacman -S mariadb-libs\nGentoo - emerge dev-db/mysql (package.use: -server -perl)\nUbuntu - apt install libmysqlclient-dev\n")),(0,a.kt)("pre",null,(0,a.kt)("code",{parentName:"pre",className:"language-text",metastring:"title='range-v3 library (header only)'",title:"'range-v3",library:!0,"(header":!0,"only)'":!0},"Arch   - pacman -S range-v3\nGentoo - emerge dev-cpp/range-v3\nUbuntu - apt install librange-v3-dev\n")),(0,a.kt)("pre",null,(0,a.kt)("code",{parentName:"pre",className:"language-text",metastring:"title='ccache'",title:"'ccache'"},"Arch   - pacman -S ccache\nGentoo - emerge dev-util/ccache\nUbuntu - apt install ccache\n"))))}c.isMDXComponent=!0}}]);