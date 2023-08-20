"use strict";(self.webpackChunktinyorm_org=self.webpackChunktinyorm_org||[]).push([[966],{3905:(e,t,n)=>{n.d(t,{Zo:()=>d,kt:()=>k});var i=n(7294);function r(e,t,n){return t in e?Object.defineProperty(e,t,{value:n,enumerable:!0,configurable:!0,writable:!0}):e[t]=n,e}function l(e,t){var n=Object.keys(e);if(Object.getOwnPropertySymbols){var i=Object.getOwnPropertySymbols(e);t&&(i=i.filter((function(t){return Object.getOwnPropertyDescriptor(e,t).enumerable}))),n.push.apply(n,i)}return n}function a(e){for(var t=1;t<arguments.length;t++){var n=null!=arguments[t]?arguments[t]:{};t%2?l(Object(n),!0).forEach((function(t){r(e,t,n[t])})):Object.getOwnPropertyDescriptors?Object.defineProperties(e,Object.getOwnPropertyDescriptors(n)):l(Object(n)).forEach((function(t){Object.defineProperty(e,t,Object.getOwnPropertyDescriptor(n,t))}))}return e}function o(e,t){if(null==e)return{};var n,i,r=function(e,t){if(null==e)return{};var n,i,r={},l=Object.keys(e);for(i=0;i<l.length;i++)n=l[i],t.indexOf(n)>=0||(r[n]=e[n]);return r}(e,t);if(Object.getOwnPropertySymbols){var l=Object.getOwnPropertySymbols(e);for(i=0;i<l.length;i++)n=l[i],t.indexOf(n)>=0||Object.prototype.propertyIsEnumerable.call(e,n)&&(r[n]=e[n])}return r}var p=i.createContext({}),s=function(e){var t=i.useContext(p),n=t;return e&&(n="function"==typeof e?e(t):a(a({},t),e)),n},d=function(e){var t=s(e.components);return i.createElement(p.Provider,{value:t},e.children)},m="mdxType",u={inlineCode:"code",wrapper:function(e){var t=e.children;return i.createElement(i.Fragment,{},t)}},c=i.forwardRef((function(e,t){var n=e.components,r=e.mdxType,l=e.originalType,p=e.parentName,d=o(e,["components","mdxType","originalType","parentName"]),m=s(n),c=r,k=m["".concat(p,".").concat(c)]||m[c]||u[c]||l;return n?i.createElement(k,a(a({ref:t},d),{},{components:n})):i.createElement(k,a({ref:t},d))}));function k(e,t){var n=arguments,r=t&&t.mdxType;if("string"==typeof e||r){var l=n.length,a=new Array(l);a[0]=c;var o={};for(var p in t)hasOwnProperty.call(t,p)&&(o[p]=t[p]);o.originalType=e,o[m]="string"==typeof e?e:r,a[1]=o;for(var s=2;s<l;s++)a[s]=n[s];return i.createElement.apply(null,a)}return i.createElement.apply(null,n)}c.displayName="MDXCreateElement"},4742:(e,t,n)=>{n.r(t),n.d(t,{assets:()=>p,contentTitle:()=>a,default:()=>u,frontMatter:()=>l,metadata:()=>o,toc:()=>s});var i=n(7462),r=(n(7294),n(3905));const l={sidebar_position:2,sidebar_label:"\ud83d\ude80 Supported Compilers",hide_table_of_contents:!0,description:"Platform requirements, supported compilers and build systems for TinyORM C++ library.",keywords:["c++ orm","supported compilers","supported build systems","tinyorm"]},a="Supported Compilers",o={unversionedId:"supported-compilers",id:"supported-compilers",title:"Supported Compilers",description:"Platform requirements, supported compilers and build systems for TinyORM C++ library.",source:"@site/docs/supported-compilers.mdx",sourceDirName:".",slug:"/supported-compilers",permalink:"/supported-compilers",draft:!1,editUrl:"https://github.com/silverqx/TinyORM-github.io/edit/main/docs/supported-compilers.mdx",tags:[],version:"current",sidebarPosition:2,frontMatter:{sidebar_position:2,sidebar_label:"\ud83d\ude80 Supported Compilers",hide_table_of_contents:!0,description:"Platform requirements, supported compilers and build systems for TinyORM C++ library.",keywords:["c++ orm","supported compilers","supported build systems","tinyorm"]},sidebar:"tinyormSidebar",previous:{title:"\ud83d\udd27 Dependencies",permalink:"/dependencies"},next:{title:"Getting Started",permalink:"/database/getting-started"}},p={},s=[{value:"<code>Windows &gt;=10</code>",id:"windows-10",level:4},{value:"<code>Linux</code>",id:"linux",level:4},{value:"Supported build systems",id:"supported-build-systems",level:2},{value:"Make tools",id:"make-tools",level:5},{value:"Parallel building",id:"parallel-building",level:5}],d={toc:s},m="wrapper";function u(e){let{components:t,...n}=e;return(0,r.kt)(m,(0,i.Z)({},d,n,{components:t,mdxType:"MDXLayout"}),(0,r.kt)("h1",{id:"supported-compilers"},"Supported Compilers"),(0,r.kt)("p",null,"Following compilers are backed up by the GitHub Action ",(0,r.kt)("a",{parentName:"p",href:"https://github.com/silverqx/TinyORM/tree/main/.github/workflows"},"workflows")," (CI pipelines), these workflows also include more then ",(0,r.kt)("strong",{parentName:"p"},"3263 unit tests")," \ud83d\ude2e\ud83d\udca5."),(0,r.kt)("div",{id:"supported-compilers"},(0,r.kt)("h4",{id:"windows-10"},(0,r.kt)("inlineCode",{parentName:"h4"},"Windows >=10")),(0,r.kt)("ul",null,(0,r.kt)("li",{parentName:"ul"},"MSVC 2019 ",(0,r.kt)("inlineCode",{parentName:"li"},">=16.9")),(0,r.kt)("li",{parentName:"ul"},"MSVC 2022 ",(0,r.kt)("inlineCode",{parentName:"li"},">=17")),(0,r.kt)("li",{parentName:"ul"},"MSYS2 UCRT64 GCC ",(0,r.kt)("inlineCode",{parentName:"li"},"10.2")," - ",(0,r.kt)("inlineCode",{parentName:"li"},"12.2")),(0,r.kt)("li",{parentName:"ul"},"MSYS2 UCRT64 Clang ",(0,r.kt)("inlineCode",{parentName:"li"},">=15")),(0,r.kt)("li",{parentName:"ul"},"clang-cl ",(0,r.kt)("inlineCode",{parentName:"li"},">=15")," with MSVC 2019/2022")),(0,r.kt)("h4",{id:"linux"},(0,r.kt)("inlineCode",{parentName:"h4"},"Linux")),(0,r.kt)("ul",null,(0,r.kt)("li",{parentName:"ul"},"GCC ",(0,r.kt)("inlineCode",{parentName:"li"},"10.2")," - ",(0,r.kt)("inlineCode",{parentName:"li"},"12.2")),(0,r.kt)("li",{parentName:"ul"},"Clang ",(0,r.kt)("inlineCode",{parentName:"li"},">=15")," ",(0,r.kt)("small",{className:"darker"},"(libstdc++ only)")))),(0,r.kt)("admonition",{type:"tip"},(0,r.kt)("p",{parentName:"admonition"},"You can compile TinyORM with the MSVC 2022 even if Qt doesn't provide binaries for the MSVC 2022, you can link against Qt MSVC 2019 binaries without any limitations.")),(0,r.kt)("admonition",{type:"caution"},(0,r.kt)("p",{parentName:"admonition"},"The ",(0,r.kt)("inlineCode",{parentName:"p"},"macOS")," and Clang with ",(0,r.kt)("inlineCode",{parentName:"p"},"libc++")," are not supported.")),(0,r.kt)("h2",{id:"supported-build-systems"},"Supported build systems"),(0,r.kt)("ul",null,(0,r.kt)("li",{parentName:"ul"},(0,r.kt)("inlineCode",{parentName:"li"},"CMake")," ",(0,r.kt)("inlineCode",{parentName:"li"},">=3.22")," ",(0,r.kt)("small",{className:"darker"},"(policies <= CMP0128 default to NEW)")),(0,r.kt)("li",{parentName:"ul"},(0,r.kt)("inlineCode",{parentName:"li"},"qmake")," distributed by the Qt Framework")),(0,r.kt)("h5",{id:"make-tools"},"Make tools"),(0,r.kt)("ul",null,(0,r.kt)("li",{parentName:"ul"},(0,r.kt)("inlineCode",{parentName:"li"},"jom")," - highly recommended with the ",(0,r.kt)("inlineCode",{parentName:"li"},"qmake")," build system on Windows ",(0,r.kt)("small",{className:"darker"},"(replacement for nmake)")),(0,r.kt)("li",{parentName:"ul"},(0,r.kt)("inlineCode",{parentName:"li"},"ninja")," - recommended for ",(0,r.kt)("inlineCode",{parentName:"li"},"CMake")," as the make file generator")),(0,r.kt)("h5",{id:"parallel-building"},"Parallel building"),(0,r.kt)("p",null,"You can control parallel building using the following environment variables."),(0,r.kt)("ul",null,(0,r.kt)("li",{parentName:"ul"},"CMake - ",(0,r.kt)("inlineCode",{parentName:"li"},"CMAKE_BUILD_PARALLEL_LEVEL")," eg. to ",(0,r.kt)("inlineCode",{parentName:"li"},"10")),(0,r.kt)("li",{parentName:"ul"},"jom - ",(0,r.kt)("inlineCode",{parentName:"li"},"JOMFLAGS")," eg. to ",(0,r.kt)("inlineCode",{parentName:"li"},"j11")),(0,r.kt)("li",{parentName:"ul"},"make - ",(0,r.kt)("inlineCode",{parentName:"li"},"MAKEFLAGS")," eg. to ",(0,r.kt)("inlineCode",{parentName:"li"},"-j10")),(0,r.kt)("li",{parentName:"ul"},"vcpkg - ",(0,r.kt)("inlineCode",{parentName:"li"},"VCPKG_MAX_CONCURRENCY")," eg. to ",(0,r.kt)("inlineCode",{parentName:"li"},"10"))))}u.isMDXComponent=!0}}]);