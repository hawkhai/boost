<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

<!--
  Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
-->

<xsl:output method="text"/>
<xsl:strip-space elements="*"/>


<xsl:variable name="newline">
<xsl:text>
</xsl:text>
</xsl:variable>


<!--
  Loop over all top-level documentation elements (i.e. classes, functions,
  variables and typedefs at namespace scope). The elements are sorted by name.
  Anything in a "detail" namespace or with "_handler" in the name is skipped.
-->
<xsl:template match="/doxygen">
<xsl:text>[/
 / Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
 /
 / Distributed under the Boost Software License, Version 1.0. (See accompanying
 / file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 /]

[section:reference Reference]

[xinclude quickref.xml]

[include requirements/asynchronous_operations.qbk]
[include requirements/AcceptHandler.qbk]
[include requirements/AsyncReadStream.qbk]
[include requirements/AsyncWriteStream.qbk]
[include requirements/CompletionHandler.qbk]
[include requirements/ConnectHandler.qbk]
[include requirements/ConstBufferSequence.qbk]
[include requirements/ConvertibleToConstBuffer.qbk]
[include requirements/ConvertibleToMutableBuffer.qbk]
[include requirements/DatagramSocketService.qbk]
[include requirements/Endpoint.qbk]
[include requirements/GettableSocketOption.qbk]
[include requirements/Handler.qbk]
[include requirements/InternetProtocol.qbk]
[include requirements/IoControlCommand.qbk]
[include requirements/IoObjectService.qbk]
[include requirements/MutableBufferSequence.qbk]
[include requirements/Protocol.qbk]
[include requirements/ReadHandler.qbk]
[include requirements/ResolveHandler.qbk]
[include requirements/ResolverService.qbk]
[include requirements/Service.qbk]
[include requirements/SettableSocketOption.qbk]
[include requirements/SocketAcceptorService.qbk]
[include requirements/SocketService.qbk]
[include requirements/StreamSocketService.qbk]
[include requirements/SyncReadStream.qbk]
[include requirements/SyncWriteStream.qbk]
[include requirements/TimeTraits.qbk]
[include requirements/TimerService.qbk]
[include requirements/WaitHandler.qbk]
[include requirements/WriteHandler.qbk]

</xsl:text>

  <xsl:for-each select="
      compounddef[@kind = 'class' or @kind = 'struct'] |
      compounddef[@kind = 'namespace']/sectiondef[1]/memberdef">
    <xsl:sort select="concat((. | ancestor::*)/compoundname, '::', name, ':x')"/>
    <xsl:sort select="name"/>
    <xsl:choose>
      <xsl:when test="@kind='class' or @kind='struct'">
        <xsl:if test="
            contains(compoundname, 'asio::') and
            not(contains(compoundname, '::detail')) and
            not(contains(compoundname, '_handler'))">
          <xsl:call-template name="class"/>
        </xsl:if>
      </xsl:when>
      <xsl:otherwise>
        <xsl:if test="not(contains(ancestor::*/compoundname, '::detail'))">
          <xsl:call-template name="namespace-memberdef"/>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:for-each>

  <xsl:value-of select="$newline"/>
  <xsl:text>[endsect]</xsl:text>

</xsl:template>


<!--========== Utilities ==========-->

<xsl:template name="strip-asio-ns">
  <xsl:param name="name"/>
  <xsl:choose>
    <xsl:when test="contains($name, 'boost::system::is_error_code_enum')">
      <xsl:value-of select="$name"/>
    </xsl:when>
    <xsl:when test="contains($name, 'asio::')">
      <xsl:value-of select="substring-after($name, 'asio::')"/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:value-of select="$name"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>


<xsl:template name="strip-ns">
  <xsl:param name="name"/>
  <xsl:choose>
    <xsl:when test="contains($name, 'boost::system::is_error_code_enum')">
      <xsl:value-of select="$name"/>
    </xsl:when>
    <xsl:when test="contains($name, '::') and contains($name, '&lt;')">
      <xsl:choose>
        <xsl:when test="string-length(substring-before($name, '::')) &lt; string-length(substring-before($name, '&lt;'))">
          <xsl:call-template name="strip-ns">
            <xsl:with-param name="name" select="substring-after($name, '::')"/>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$name"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:when>
    <xsl:when test="contains($name, '::')">
      <xsl:call-template name="strip-ns">
        <xsl:with-param name="name" select="substring-after($name, '::')"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:otherwise>
      <xsl:value-of select="$name"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>


<xsl:template name="make-id">
  <xsl:param name="name"/>
  <xsl:choose>
    <xsl:when test="contains($name, 'boost::system::')">
      <xsl:call-template name="make-id">
        <xsl:with-param name="name"
         select="substring-after($name, 'boost::system::')"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:when test="contains($name, 'boost::asio::error::')">
      <xsl:call-template name="make-id">
        <xsl:with-param name="name"
         select="concat(substring-before($name, 'boost::asio::error::'), substring-after($name, 'boost::asio::error::'))"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:when test="contains($name, '::')">
      <xsl:call-template name="make-id">
        <xsl:with-param name="name"
         select="concat(substring-before($name, '::'), '__', substring-after($name, '::'))"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:when test="contains($name, '=')">
      <xsl:call-template name="make-id">
        <xsl:with-param name="name"
         select="concat(substring-before($name, '='), '_eq_', substring-after($name, '='))"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:when test="contains($name, '!')">
      <xsl:call-template name="make-id">
        <xsl:with-param name="name"
         select="concat(substring-before($name, '!'), '_not_', substring-after($name, '!'))"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:when test="contains($name, '&lt;')">
      <xsl:call-template name="make-id">
        <xsl:with-param name="name"
         select="concat(substring-before($name, '&lt;'), '_lt_', substring-after($name, '&lt;'))"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:when test="contains($name, '&gt;')">
      <xsl:call-template name="make-id">
        <xsl:with-param name="name"
         select="concat(substring-before($name, '&gt;'), '_gt_', substring-after($name, '&gt;'))"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:when test="contains($name, '~')">
      <xsl:call-template name="make-id">
        <xsl:with-param name="name"
         select="concat(substring-before($name, '~'), '_', substring-after($name, '~'))"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:when test="contains($name, ' ')">
      <xsl:call-template name="make-id">
        <xsl:with-param name="name"
         select="concat(substring-before($name, ' '), '_', substring-after($name, ' '))"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:when test="contains($name, 'boost__posix_time__ptime')">
      <xsl:call-template name="make-id">
        <xsl:with-param name="name"
         select="concat(substring-before($name, 'boost__posix_time__ptime'), 'ptime', substring-after($name, 'boost__posix_time__ptime'))"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:otherwise>
      <xsl:value-of select="$name"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>


<!--========== Markup ==========-->

<xsl:template match="para" mode="markup">
<xsl:text>
</xsl:text><xsl:apply-templates mode="markup"/><xsl:text>
</xsl:text>
</xsl:template>


<xsl:template match="para" mode="markup-nested">
<xsl:apply-templates mode="markup-nested"/>
</xsl:template>


<xsl:template match="ref" mode="markup">
<xsl:apply-templates mode="markup-nested"/>
</xsl:template>


<xsl:template match="title" mode="markup">
  <xsl:variable name="title">
    <xsl:value-of select="."/>
  </xsl:variable>
  <xsl:if test="string-length($title) > 0">
[heading <xsl:value-of select="."/>]
  </xsl:if>
</xsl:template>


<xsl:template match="programlisting" mode="markup">
  <xsl:value-of select="$newline"/>
  <xsl:value-of select="$newline"/>
  <xsl:apply-templates mode="codeline"/>
  <xsl:value-of select="$newline"/>
  <xsl:value-of select="$newline"/>
</xsl:template>


<xsl:template match="programlisting" mode="markup-nested">
  <xsl:value-of select="$newline"/>
  <xsl:text>``</xsl:text>
  <xsl:value-of select="$newline"/>
  <xsl:apply-templates mode="codeline"/>
  <xsl:value-of select="$newline"/>
  <xsl:text>``</xsl:text>
  <xsl:value-of select="$newline"/>
</xsl:template>


<xsl:template match="codeline" mode="codeline">
  <xsl:if test="string-length(.) &gt; 0">
    <xsl:text>  </xsl:text>
  </xsl:if>
  <xsl:apply-templates mode="codeline"/>
  <xsl:value-of select="$newline"/>
</xsl:template>


<xsl:template match="sp" mode="markup">
<xsl:text> </xsl:text>
</xsl:template>


<xsl:template match="sp" mode="markup-nested">
<xsl:text> </xsl:text>
</xsl:template>


<xsl:template match="sp" mode="codeline">
  <xsl:text> </xsl:text>
</xsl:template>


<xsl:template match="linebreak" mode="markup">
<xsl:text>

</xsl:text>
</xsl:template>


<xsl:template match="linebreak" mode="markup-nested">
<xsl:text>

</xsl:text>
</xsl:template>


<xsl:template match="listitem" mode="markup">
* <xsl:value-of select="."/><xsl:text>
</xsl:text>
</xsl:template>


<xsl:template match="emphasis" mode="markup">[*<xsl:value-of select="."/>] </xsl:template>


<xsl:template match="parameterlist" mode="markup">
  <xsl:choose>
    <xsl:when test="@kind='param'">
[heading Parameters]
    </xsl:when>
    <xsl:when test="@kind='exception'">
[heading Exceptions]
    </xsl:when>
  </xsl:choose>

[variablelist
  <xsl:apply-templates mode="markup"/>
]
</xsl:template>


<xsl:template match="parameteritem" mode="markup">
[[<xsl:value-of select="parameternamelist"/>][<xsl:apply-templates select="parameterdescription" mode="markup-nested"/>]]
</xsl:template>


<xsl:template match="simplesect" mode="markup">
  <xsl:choose>
    <xsl:when test="@kind='return'">
[heading Return Value]
      <xsl:apply-templates mode="markup"/>
    </xsl:when>
    <xsl:when test="@kind='see'">
    </xsl:when>
    <xsl:when test="@kind='note'">
[heading Remarks]
      <xsl:apply-templates mode="markup"/>
    </xsl:when>
    <xsl:when test="@kind='par'">
      <xsl:if test="not(starts-with(title, 'Concepts:'))">
        <xsl:apply-templates mode="markup"/>
      </xsl:if>
    </xsl:when>
    <xsl:otherwise>
      <xsl:apply-templates mode="markup"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>


<xsl:template match="text()" mode="markup">
  <xsl:variable name="text" select="."/>
  <xsl:variable name="starts-with-whitespace" select="
      starts-with($text, ' ') or starts-with($text, $newline)"/>
  <xsl:variable name="preceding-node-name">
    <xsl:for-each select="preceding-sibling::*">
      <xsl:if test="position() = last()">
        <xsl:value-of select="local-name()"/>
      </xsl:if>
    </xsl:for-each>
  </xsl:variable>
  <xsl:variable name="after-newline" select="
      $preceding-node-name = 'programlisting' or
      $preceding-node-name = 'linebreak'"/>
  <xsl:choose>
    <xsl:when test="$starts-with-whitespace and $after-newline">
      <xsl:call-template name="escape-text">
        <xsl:with-param name="text">
          <xsl:call-template name="strip-leading-whitespace">
            <xsl:with-param name="text" select="$text"/>
          </xsl:call-template>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="escape-text">
        <xsl:with-param name="text" select="$text"/>
      </xsl:call-template>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>


<xsl:template match="text()" mode="markup-nested">
  <xsl:variable name="text" select="."/>
  <xsl:variable name="starts-with-whitespace" select="
      starts-with($text, ' ') or starts-with($text, $newline)"/>
  <xsl:variable name="preceding-node-name">
    <xsl:for-each select="preceding-sibling::*">
      <xsl:if test="position() = last()">
        <xsl:value-of select="local-name()"/>
      </xsl:if>
    </xsl:for-each>
  </xsl:variable>
  <xsl:variable name="after-newline" select="
      $preceding-node-name = 'programlisting' or
      $preceding-node-name = 'linebreak'"/>
  <xsl:choose>
    <xsl:when test="$starts-with-whitespace and $after-newline">
      <xsl:call-template name="escape-text">
        <xsl:with-param name="text">
          <xsl:call-template name="strip-leading-whitespace">
            <xsl:with-param name="text" select="$text"/>
          </xsl:call-template>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="escape-text">
        <xsl:with-param name="text" select="$text"/>
      </xsl:call-template>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>


<xsl:template name="strip-leading-whitespace">
  <xsl:param name="text"/>
  <xsl:choose>
    <xsl:when test="starts-with($text, ' ')">
      <xsl:call-template name="strip-leading-whitespace">
        <xsl:with-param name="text" select="substring($text, 2)"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:when test="starts-with($text, $newline)">
      <xsl:call-template name="strip-leading-whitespace">
        <xsl:with-param name="text" select="substring($text, 2)"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:otherwise>
      <xsl:value-of select="$text"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>


<xsl:template name="escape-text">
  <xsl:param name="text"/>
  <xsl:choose>
    <xsl:when test="contains($text, '_')">
      <xsl:value-of select="substring-before($text, '_')"/>
      <xsl:text>\_</xsl:text>
      <xsl:call-template name="escape-text">
        <xsl:with-param name="text" select="substring-after($text, '_')"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:otherwise>
      <xsl:value-of select="$text"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>


<xsl:template match="ref[@kindref='compound']" mode="markup">
  <xsl:variable name="name">
    <xsl:value-of select="."/>
  </xsl:variable>
  <xsl:choose>
    <xsl:when test="contains($name, 'asio::')">
      <xsl:variable name="ref-name">
        <xsl:call-template name="strip-asio-ns">
          <xsl:with-param name="name" select="$name"/>
        </xsl:call-template>
      </xsl:variable>
      <xsl:variable name="ref-id">
        <xsl:call-template name="make-id">
          <xsl:with-param name="name" select="$ref-name"/>
        </xsl:call-template>
      </xsl:variable>
[link boost_asio.reference.<xsl:value-of select="$ref-id"/><xsl:text> </xsl:text><xsl:value-of
 select="$ref-name"/>]</xsl:when>
    <xsl:otherwise><xsl:value-of select="."/></xsl:otherwise>
  </xsl:choose>
</xsl:template>


<!--========== Class ==========-->

<xsl:template name="class">
  <xsl:variable name="class-name">
    <xsl:call-template name="strip-asio-ns">
      <xsl:with-param name="name" select="compoundname"/>
    </xsl:call-template>
  </xsl:variable>
  <xsl:variable name="unqualified-class-name">
    <xsl:call-template name="strip-ns">
      <xsl:with-param name="name" select="compoundname"/>
    </xsl:call-template>
  </xsl:variable>
  <xsl:variable name="class-id">
    <xsl:call-template name="make-id">
      <xsl:with-param name="name" select="$class-name"/>
    </xsl:call-template>
  </xsl:variable>
[section:<xsl:value-of select="$class-id"/><xsl:text> </xsl:text><xsl:value-of select="$class-name"/>]

<xsl:value-of select="briefdescription"/><xsl:text>

</xsl:text>

<xsl:apply-templates select="templateparamlist"
 mode="class-detail"/><xsl:text>  </xsl:text><xsl:value-of select="@kind"/><xsl:text> </xsl:text><xsl:value-of
 select="$unqualified-class-name"/><xsl:if test="count(basecompoundref[not(contains(.,'::detail'))]) > 0"> :</xsl:if><xsl:text>
</xsl:text><xsl:for-each select="basecompoundref[not(contains(.,'::detail'))]">
<xsl:text>    </xsl:text><xsl:if test="@prot='public'">public </xsl:if><xsl:call-template
 name="strip-asio-ns"><xsl:with-param name="name" select="."/></xsl:call-template><xsl:if
 test="not(position() = last())">,</xsl:if><xsl:text>
</xsl:text></xsl:for-each><xsl:text>
</xsl:text>

<xsl:call-template name="class-tables">
  <xsl:with-param name="class-name" select="$class-name"/>
  <xsl:with-param name="class-id" select="$class-id"/>
</xsl:call-template>

<xsl:apply-templates select="detaileddescription" mode="markup"/>

<xsl:call-template name="class-members">
  <xsl:with-param name="class-name" select="$class-name"/>
  <xsl:with-param name="class-id" select="$class-id"/>
</xsl:call-template>

[endsect]
</xsl:template>


<xsl:template name="class-tables">
<xsl:param name="class-name"/>
<xsl:param name="class-id"/>
<xsl:if test="
    count(
      sectiondef[@kind='public-type'] |
      innerclass[@prot='public' and not(contains(., '_handler'))]) &gt; 0">
[heading Types]
[table
  [[Name][Description]]
<xsl:for-each select="
    sectiondef[@kind='public-type']/memberdef |
    innerclass[@prot='public' and not(contains(., '_handler'))]" mode="class-table">
  <xsl:sort select="concat(name, (.)[not(name)])"/>
  [
<xsl:choose>
  <xsl:when test="count(name) &gt; 0">
    [[link boost_asio.reference.<xsl:value-of select="$class-id"/>.<xsl:value-of select="name"/>
      <xsl:text> </xsl:text>[*<xsl:value-of select="name"/>]]]
    [<xsl:value-of select="briefdescription"/>]
  </xsl:when>
  <xsl:otherwise>
    <xsl:variable name="type-name">
      <xsl:call-template name="strip-asio-ns">
        <xsl:with-param name="name" select="."/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="unqualified-type-name">
      <xsl:call-template name="strip-ns">
        <xsl:with-param name="name" select="."/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="type-id">
      <xsl:call-template name="make-id">
        <xsl:with-param name="name" select="$type-name"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="type-ref-id" select="@refid"/>
    [[link boost_asio.reference.<xsl:value-of select="$type-id"/>
      <xsl:text> </xsl:text>[*<xsl:value-of select="$unqualified-type-name"/>]]]
    [<xsl:value-of select="(/doxygen//compounddef[@id=$type-ref-id])[1]/briefdescription"/>]
  </xsl:otherwise>
</xsl:choose>
  ]
</xsl:for-each>
]
</xsl:if>

<xsl:if test="count(sectiondef[@kind='public-func' or @kind='public-static-func']) > 0">
[heading Member Functions]
[table
  [[Name][Description]]
<xsl:for-each select="sectiondef[@kind='public-func' or @kind='public-static-func']/memberdef" mode="class-table">
  <xsl:sort select="name"/>
  <xsl:variable name="name">
    <xsl:value-of select="name"/>
  </xsl:variable>
  <xsl:variable name="id">
    <xsl:call-template name="make-id">
      <xsl:with-param name="name" select="$name"/>
    </xsl:call-template>
  </xsl:variable>
  <xsl:variable name="doxygen-id">
    <xsl:value-of select="@id"/>
  </xsl:variable>
  <xsl:variable name="overload-position">
    <xsl:for-each select="../memberdef[name = $name]">
      <xsl:if test="@id = $doxygen-id">
        <xsl:value-of select="position()"/>
      </xsl:if>
    </xsl:for-each>
  </xsl:variable>
  <xsl:if test="$overload-position = 1">
  [
    [[link boost_asio.reference.<xsl:value-of select="$class-id"/>.<xsl:value-of select="$id"/>
      <xsl:text> </xsl:text>[*<xsl:value-of select="$name"/>]]]
    [<xsl:value-of select="briefdescription"/>]
  ]
  </xsl:if>
</xsl:for-each>
]
</xsl:if>

<xsl:if test="count(sectiondef[@kind='friend']/memberdef[not(type = 'friend class')]) &gt; 0">
[heading Friends]
[table
  [[Name][Description]]
<xsl:for-each select="sectiondef[@kind='friend']/memberdef[not(type = 'friend class')]" mode="class-table">
  <xsl:sort select="name"/>
  <xsl:variable name="name">
    <xsl:value-of select="name"/>
  </xsl:variable>
  <xsl:variable name="id">
    <xsl:call-template name="make-id">
      <xsl:with-param name="name" select="$name"/>
    </xsl:call-template>
  </xsl:variable>
  <xsl:variable name="doxygen-id">
    <xsl:value-of select="@id"/>
  </xsl:variable>
  <xsl:variable name="overload-position">
    <xsl:for-each select="../memberdef[name = $name]">
      <xsl:if test="@id = $doxygen-id">
        <xsl:value-of select="position()"/>
      </xsl:if>
    </xsl:for-each>
  </xsl:variable>
  <xsl:if test="$overload-position = 1">
  [
    [[link boost_asio.reference.<xsl:value-of select="$class-id"/>.<xsl:value-of select="$id"/>
      <xsl:text> </xsl:text>[*<xsl:value-of select="$name"/>]]]
    [<xsl:value-of select="briefdescription"/>]
  ]
  </xsl:if>
</xsl:for-each>
]
</xsl:if>

<xsl:if test="count(sectiondef[@kind='public-attrib' or @kind='public-static-attrib']) > 0">
[heading Data Members]
[table
  [[Name][Description]]
<xsl:for-each select="sectiondef[@kind='public-attrib' or @kind='public-static-attrib']/memberdef" mode="class-table">
  <xsl:sort select="name"/>
  [
    [[link boost_asio.reference.<xsl:value-of select="$class-id"/>.<xsl:value-of select="name"/>
      <xsl:text> </xsl:text>[*<xsl:value-of select="name"/>]]]
    [<xsl:value-of select="briefdescription"/>]
  ]
</xsl:for-each>
]
</xsl:if>
</xsl:template>


<xsl:template name="class-members">
<xsl:param name="class-name"/>
<xsl:param name="class-id"/>
<xsl:apply-templates select="sectiondef[@kind='public-type' or @kind='public-func' or @kind='public-static-func' or @kind='public-attrib' or @kind='public-static-attrib' or @kind='friend']/memberdef[not(type = 'friend class')]" mode="class-detail">
  <xsl:sort select="name"/>
  <xsl:with-param name="class-name" select="$class-name"/>
  <xsl:with-param name="class-id" select="$class-id"/>
</xsl:apply-templates>
</xsl:template>


<!-- Class detail -->

<xsl:template match="memberdef" mode="class-detail">
  <xsl:param name="class-name"/>
  <xsl:param name="class-id"/>
  <xsl:variable name="name">
    <xsl:value-of select="name"/>
  </xsl:variable>
  <xsl:variable name="id">
    <xsl:call-template name="make-id">
      <xsl:with-param name="name" select="$name"/>
    </xsl:call-template>
  </xsl:variable>
  <xsl:variable name="doxygen-id">
    <xsl:value-of select="@id"/>
  </xsl:variable>
  <xsl:variable name="overload-count">
    <xsl:value-of select="count(../memberdef[name = $name])"/>
  </xsl:variable>
  <xsl:variable name="overload-position">
    <xsl:for-each select="../memberdef[name = $name]">
      <xsl:if test="@id = $doxygen-id">
        <xsl:value-of select="position()"/>
      </xsl:if>
    </xsl:for-each>
  </xsl:variable>

<xsl:if test="$overload-count &gt; 1 and $overload-position = 1">
[section:<xsl:value-of select="$id"/><xsl:text> </xsl:text>
<xsl:value-of select="$class-name"/>::<xsl:value-of select="$name"/>]

<xsl:value-of select="briefdescription"/><xsl:text>
</xsl:text>

<xsl:for-each select="../memberdef[name = $name]">
<xsl:text>
</xsl:text><xsl:apply-templates select="templateparamlist" mode="class-detail"/>
<xsl:text>  </xsl:text><xsl:if test="@static='yes'">static </xsl:if><xsl:if
 test="string-length(type) > 0"><xsl:value-of select="type"/><xsl:text> </xsl:text>
</xsl:if>``[link boost_asio.reference.<xsl:value-of select="$class-id"/>.<xsl:value-of
 select="$id"/>.overload<xsl:value-of select="position()"/><xsl:text> </xsl:text><xsl:value-of
 select="name"/>]``(<xsl:apply-templates select="param"
 mode="class-detail"/>)<xsl:if test="@const='yes'"> const</xsl:if>;
</xsl:for-each>
</xsl:if>

[section:<xsl:if test="$overload-count = 1"><xsl:value-of select="$id"/></xsl:if>
<xsl:if test="$overload-count &gt; 1">overload<xsl:value-of select="$overload-position"/></xsl:if>
<xsl:text> </xsl:text><xsl:value-of select="$class-name"/>::<xsl:value-of select="$name"/>
<xsl:if test="$overload-count &gt; 1"> (<xsl:value-of
 select="$overload-position"/> of <xsl:value-of select="$overload-count"/> overloads)</xsl:if>]

<xsl:if test="not(starts-with($doxygen-id, ../../@id))">
<xsl:variable name="inherited-from" select="/doxygen/compounddef[starts-with($doxygen-id, @id)]/compoundname"/>
<xsl:if test="not(contains($inherited-from, '::detail'))">
['Inherited from <xsl:call-template name="strip-asio-ns">
<xsl:with-param name="name" select="$inherited-from"/>
</xsl:call-template>.]<xsl:text>

</xsl:text></xsl:if></xsl:if>

<xsl:value-of select="briefdescription"/><xsl:text>
</xsl:text>

  <xsl:choose>
    <xsl:when test="@kind='typedef'">
      <xsl:call-template name="typedef" mode="class-detail">
        <xsl:with-param name="class-name" select="$class-name"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:when test="@kind='variable'">
      <xsl:call-template name="variable" mode="class-detail"/>
    </xsl:when>
    <xsl:when test="@kind='enum'">
      <xsl:call-template name="enum" mode="class-detail"/>
    </xsl:when>
    <xsl:when test="@kind='function'">
      <xsl:call-template name="function" mode="class-detail"/>
    </xsl:when>
    <xsl:when test="@kind='friend'">
      <xsl:call-template name="function" mode="class-detail"/>
    </xsl:when>
  </xsl:choose>

<xsl:text>
</xsl:text><xsl:apply-templates select="detaileddescription" mode="markup"/>

[endsect]

<xsl:if test="$overload-count &gt; 1 and $overload-position = $overload-count">
[endsect]
</xsl:if>
</xsl:template>


<xsl:template name="typedef">
<xsl:param name="class-name"/>
<xsl:text>
  </xsl:text>typedef <xsl:value-of select="type"/><xsl:text> </xsl:text><xsl:value-of select="name"/>;<xsl:text>

</xsl:text>
<xsl:if test="count(type/ref) &gt; 0 and not(contains(type, '*'))">
  <xsl:variable name="class-refid">
    <xsl:for-each select="type/ref[1]">
      <xsl:value-of select="@refid"/>
    </xsl:for-each>
  </xsl:variable>
  <xsl:variable name="name" select="name"/>
  <xsl:for-each select="/doxygen/compounddef[@id=$class-refid]">
    <xsl:call-template name="class-tables">
      <xsl:with-param name="class-name">
        <xsl:value-of select="concat($class-name, '::', $name)"/>
      </xsl:with-param>
      <xsl:with-param name="class-id">
        <xsl:call-template name="make-id">
          <xsl:with-param name="name">
            <xsl:call-template name="strip-asio-ns">
              <xsl:with-param name="name" select="compoundname"/>
            </xsl:call-template>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:apply-templates select="detaileddescription" mode="markup"/>
  </xsl:for-each>
</xsl:if>
</xsl:template>


<xsl:template name="variable">
<xsl:text>
  </xsl:text><xsl:if test="@static='yes'">static </xsl:if><xsl:value-of
 select="type"/><xsl:text> </xsl:text><xsl:value-of select="name"/>
 <xsl:if test="count(initializer) = 1"><xsl:text> =</xsl:text>
 <xsl:value-of select="initializer"/></xsl:if>;
</xsl:template>


<xsl:template name="enum">
  enum <xsl:value-of select="name"/>

[heading Values]
[variablelist
<xsl:for-each select="enumvalue">
  [
    [<xsl:value-of select="name"/>]
    [<xsl:value-of select="briefdescription"/>]
  ]
</xsl:for-each>
]
</xsl:template>


<xsl:template name="function">
<xsl:text>
</xsl:text><xsl:apply-templates select="templateparamlist" mode="class-detail"/>
<xsl:text>  </xsl:text><xsl:if test="@static='yes'">static </xsl:if><xsl:if
 test="string-length(type) > 0"><xsl:value-of select="type"/><xsl:text> </xsl:text></xsl:if>
<xsl:value-of select="name"/>(<xsl:apply-templates select="param"
 mode="class-detail"/>)<xsl:if test="@const='yes'"> const</xsl:if>;
</xsl:template>


<xsl:template match="templateparamlist" mode="class-detail">
<xsl:text>  </xsl:text>template&lt;<xsl:apply-templates select="param" mode="class-detail-template"/>&gt;
</xsl:template>


<xsl:template match="param" mode="class-detail-template">
<xsl:text>
      </xsl:text><xsl:value-of select="type"/><xsl:text> </xsl:text>
      <xsl:choose>
        <xsl:when test="declname = 'Allocator'">
          <xsl:value-of select="declname"/>
        </xsl:when>
        <xsl:when test="declname = 'Arg'">
          <xsl:value-of select="declname"/>
        </xsl:when>
        <xsl:when test="declname = 'CompletionCondition'">
          <xsl:value-of select="declname"/>
        </xsl:when>
        <xsl:when test="declname = 'Context_Service'">
          <xsl:value-of select="declname"/>
        </xsl:when>
        <xsl:when test="declname = 'Function'">
          <xsl:value-of select="declname"/>
        </xsl:when>
        <xsl:when test="declname = 'HandshakeHandler'">
          <xsl:value-of select="declname"/>
        </xsl:when>
        <xsl:when test="declname = 'N'">
          <xsl:value-of select="declname"/>
        </xsl:when>
        <xsl:when test="declname = 'PasswordCallback'">
          <xsl:value-of select="declname"/>
        </xsl:when>
        <xsl:when test="declname = 'PodType'">
          <xsl:value-of select="declname"/>
        </xsl:when>
        <xsl:when test="declname = 'ShutdownHandler'">
          <xsl:value-of select="declname"/>
        </xsl:when>
        <xsl:when test="declname = 'Stream'">
          <xsl:value-of select="declname"/>
        </xsl:when>
        <xsl:when test="declname = 'T'">
          <xsl:value-of select="declname"/>
        </xsl:when>
        <xsl:when test="declname = 'T1'">
          <xsl:value-of select="declname"/>
        </xsl:when>
        <xsl:when test="declname = 'TN'">
          <xsl:value-of select="declname"/>
        </xsl:when>
        <xsl:when test="declname = 'Time'">
          <xsl:value-of select="declname"/>
        </xsl:when>
        <xsl:when test="count(declname) = 0">
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="concat('``[link boost_asio.reference.', declname, ' ', declname, ']``')"/>
        </xsl:otherwise>
      </xsl:choose>
      <xsl:if test="count(defval) > 0"> = <xsl:value-of
        select="defval"/></xsl:if><xsl:if test="not(position() = last())">,</xsl:if>
</xsl:template>


<xsl:template match="param" mode="class-detail">
<xsl:text>
      </xsl:text><xsl:value-of select="type"/><xsl:text> </xsl:text><xsl:value-of
        select="declname"/><xsl:if test="count(defval) > 0"> = <xsl:value-of
        select="defval"/></xsl:if><xsl:if test="not(position() = last())">,</xsl:if>
</xsl:template>


<xsl:template match="*" mode="class-detail"/>


<!--========== Namespace ==========-->

<xsl:template name="namespace-memberdef">
  <xsl:variable name="name">
    <xsl:call-template name="strip-asio-ns">
      <xsl:with-param name="name" select="concat(../../compoundname, '::', name)"/>
    </xsl:call-template>
  </xsl:variable>
  <xsl:variable name="unqualified-name">
    <xsl:call-template name="strip-ns">
      <xsl:with-param name="name" select="$name"/>
    </xsl:call-template>
  </xsl:variable>
  <xsl:variable name="id">
    <xsl:call-template name="make-id">
      <xsl:with-param name="name" select="$name"/>
    </xsl:call-template>
  </xsl:variable>
  <xsl:variable name="doxygen-id">
    <xsl:value-of select="@id"/>
  </xsl:variable>
  <xsl:variable name="overload-count">
    <xsl:value-of select="count(../memberdef[name = $unqualified-name])"/>
  </xsl:variable>
  <xsl:variable name="overload-position">
    <xsl:for-each select="../memberdef[name = $unqualified-name]">
      <xsl:if test="@id = $doxygen-id">
        <xsl:value-of select="position()"/>
      </xsl:if>
    </xsl:for-each>
  </xsl:variable>

<xsl:if test="$overload-count &gt; 1 and $overload-position = 1">
[section:<xsl:value-of select="$id"/><xsl:text> </xsl:text><xsl:value-of select="$name"/>]

<xsl:value-of select="briefdescription"/><xsl:text>
</xsl:text>

<xsl:for-each select="../memberdef[name = $unqualified-name]">
<xsl:text>
</xsl:text><xsl:apply-templates select="templateparamlist" mode="class-detail"/>
<xsl:text>  </xsl:text><xsl:if test="string-length(type) > 0"><xsl:value-of
 select="type"/><xsl:text> </xsl:text></xsl:if>``[link boost_asio.reference.<xsl:value-of
 select="$id"/>.overload<xsl:value-of select="position()"/><xsl:text> </xsl:text>
<xsl:value-of select="name"/>]``(<xsl:apply-templates select="param" mode="class-detail"/>);
</xsl:for-each>

<xsl:for-each select="/doxygen/compounddef[@kind='group' and compoundname=$name]">
  <xsl:apply-templates select="detaileddescription" mode="markup"/>
</xsl:for-each>

</xsl:if>

[section:<xsl:if test="$overload-count = 1"><xsl:value-of select="$id"/></xsl:if>
<xsl:if test="$overload-count &gt; 1">overload<xsl:value-of select="$overload-position"/>
</xsl:if><xsl:text> </xsl:text><xsl:value-of select="$name"/>
<xsl:if test="$overload-count &gt; 1"> (<xsl:value-of
 select="$overload-position"/> of <xsl:value-of select="$overload-count"/> overloads)</xsl:if>]

<xsl:value-of select="briefdescription"/><xsl:text>
</xsl:text>

  <xsl:choose>
    <xsl:when test="@kind='typedef'">
      <xsl:call-template name="typedef">
        <xsl:with-param name="class-name" select="$name"/>
      </xsl:call-template>
    </xsl:when>
    <xsl:when test="@kind='variable'">
      <xsl:call-template name="variable"/>
    </xsl:when>
    <xsl:when test="@kind='enum'">
      <xsl:call-template name="enum"/>
    </xsl:when>
    <xsl:when test="@kind='function'">
      <xsl:call-template name="function"/>
    </xsl:when>
  </xsl:choose>

<xsl:text>
</xsl:text><xsl:apply-templates select="detaileddescription" mode="markup"/>

[endsect]

<xsl:if test="$overload-count &gt; 1 and $overload-position = $overload-count">
[endsect]
</xsl:if>
</xsl:template>


</xsl:stylesheet>
