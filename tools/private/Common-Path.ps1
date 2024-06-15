#!/usr/bin/env pwsh

Set-StrictMode -Version 3.0

# Get slashes by platform (all allowed platform dependant path separators)
function Get-Slashes {
    [OutputType([string])]
    Param(
        [Parameter(HelpMessage = 'Specifies whether to escape slashes for RegEx.')]
        [switch] $ForRegEx
    )

    # Test precondition only if the $Script:Slashes is defined
    if (Test-Path 'Variable:Script:Slashes') {
        # Test correct type
        if ($null -ne $Script:Slashes -and $Script:Slashes -isnot [string]) {
            throw 'The $Script:Slashes variable must be [string] or $null, the current type is [' +
                $Script:Slashes.GetType() + '].'
        }

        # Cached value
        if (-not [string]::IsNullOrEmpty($Script:Slashes)) {
            return $Script:Slashes
        }
    }

    $platform = $PSVersionTable.Platform

    switch ($platform) {
        'Win32NT' {
            $slashes = '\/'
            return $Script:Slashes = $ForRegEx ? [regex]::Escape($slashes) : $slashes
        }
        'Unix' {
            return $Script:Slashes = '/'
        }
        Default {
            throw "$platform platform is not supported."
        }
    }
}

# Get the RegEx for a given path that can be used to determine if the given path is
# on the system path ($env:Path or $PATH).
# It can also be used to match only one path because the RegEx starts/ends like this:
# (?:^|;)...(?:$|;)
# It's cross-platform. It doesn't normalize the driver letter on Windows,
# it normalizes path slashes only.
# Generates the RegEx something like the following:
# (?:^|;)E:[\\/]+dotfiles[\\/]+bin[\\/]*(?:$|;)
function Get-PathToMatch {
    [OutputType([string])]
    Param(
        [Parameter(Position = 0, Mandatory, ValueFromPipeline, ValueFromPipelinebyPropertyName,
            HelpMessage = 'Specifies the path for which the match pattern is to be returned.')]
        [ValidateNotNullOrEmpty()]
        [string]
        $Path
    )

    $slashes = Get-Slashes -ForRegEx

    # Normalize path slashes
    $pathToMatch = $Path `
        -replace "[$slashes]+(?![$slashes]*$)", "[$slashes]+" `
        -replace "[$slashes]+$", ''

    $separator = [IO.Path]::PathSeparator

    return "(?:^|$separator)$pathToMatch[$slashes]*(?:$|$separator)"
}

# Removed trailing slashes from the given path (relative or absolute)
function Remove-TrailingSlashes {
    [CmdletBinding()]
    [OutputType([string[]])]
    Param(
        [Parameter(Position = 0, Mandatory, ValueFromPipeline, ValueFromPipelinebyPropertyName,
            HelpMessage = 'Specifies a path for which to remove trailing slashes.')]
        [AllowEmptyCollection()]
        [string[]]
        $Path
    )

    process {
        [string[]] $normalized = @()

        foreach ($pathRaw in $Path) {
            $normalized += $pathRaw -replace "[$(Get-Slashes -ForRegEx)]+$", ''
        }

        return $normalized
    }
}

# Remove a path separator at the end and call the [System.IO.Path]::GetFullPath (normalize to \)
function Get-FullPath {
    [CmdletBinding()]
    [OutputType([string[]])]
    Param(
        [Parameter(Position = 0, Mandatory, ValueFromPipeline, ValueFromPipelinebyPropertyName,
            HelpMessage = 'Specifies a path to normalize.')]
        [AllowEmptyCollection()]
        [string[]]
        $Path,

        [Parameter(ValueFromPipelinebyPropertyName,
            HelpMessage = 'Specifies the beginning of a fully qualified path for the given ' +
                'relative Path.')]
        [string]
        $BasePath
    )

    process {
        [string[]] $normalized = @()

        foreach ($pathRaw in $Path) {
            # Removed trailing slashes
            $pathNoTrailingSlashes = Remove-TrailingSlashes -Path $pathRaw

                           # Needed to select the correct overload
            $normalized += $PSBoundParameters.ContainsKey('BasePath') `
                           ? [System.IO.Path]::GetFullPath($pathNoTrailingSlashes, $BasePath)
                           : [System.IO.Path]::GetFullPath($pathNoTrailingSlashes)
        }

        return $normalized
    }
}
